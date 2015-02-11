//=============================================================================
//	AIクラス
//=============================================================================
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "AI.h"
#include<float.h>
#include <math.h>

//------------------------------------------------------------------------------
//マクロ定義
//------------------------------------------------------------------------------
// 角関連
#define PI     (3.141592654f) // 円周率
#define RAD_TO_DEG   (57.29577951f) // 1ラジアン→度
#define DEG_TO_RAD   (0.017453293f) // 1度→ラジアン

// 度→ラジアン
#define DEG2RAD(degree)  ((degree) * DEG_TO_RAD)
// ラジアン→度
#define RAD2DEG(radian)  ((radian) * RAD_TO_DEG)

// 角度の正規化(PI～-PI)
#define REVISE_PI(radian) {if((radian) > PI){(radian) -= (2.0f * PI);}\
								else if((radian) < -PI){(radian) += (2.0f * PI);}}
//------------------------------------------------------------------------------
//定数定義
//------------------------------------------------------------------------------
const float PLAYER_MOVE_SPEED (0.5f);			// 移動にかかる係数
const int UserMax = 6;//最大ユーザー数
const double SURCH_MAX = 3000.0f*3000.0f;	//フィールドのサイズの1/4
const float GRAVITY = -0.08f;				//弾にかかる重力加速度
const float BULLET_SPEED = 10.0f;			//弾の速度
const float TURN_SPEED = DEG2RAD(3.0f);		//回転速度
const int PLAYER_RELOAD_TIME (300);			// リロードまでのフレーム
//------------------------------------------------------------------------------
//静的メンバ変数定義
//------------------------------------------------------------------------------
AI* AI::Top = nullptr;
AI* AI::Cur = nullptr;

float leap(float start,float end,float delta)
{
	return (start*(1.0f-delta) + end*delta);
}
//------------------------------------------------------------------------------
//リストに追加
//------------------------------------------------------------------------------
void AI::LinkList(void)
{
	if (Top != NULL)//二つ目以降の処理
	{
		AI* ai = Cur;
		ai->Next = this;
		Prev = ai;
		Next = NULL;
		Cur = this;
	}
	else//最初の一つの時の処理
	{
		Top = this;
		Cur = this;
		Prev = NULL;
		Next = NULL;
	}
}

//------------------------------------------------------------------------------
//リストから削除
//------------------------------------------------------------------------------
void AI::UnlinkList(void)
{
	if (Prev == NULL)//先頭
	{
		if (Next != NULL)//次がある
		{
			Next->Prev = NULL;
			Top = Next;
		}
		else//最後の一つだった
		{
			Top = NULL;
			Cur = NULL;
		}
	}
	else if (Next == NULL)//終端
	{
		if (Prev != NULL)//前がある
		{
			Prev->Next = NULL;
			Cur = Prev;
		}
		else//最後の一つだった
		{
			Top = NULL;
			Cur = NULL;
		}
	}
	else//前後にデータがあるとき
	{
		Prev->Next = Next;
		Next->Prev = Prev;
	}

	Prev = NULL;
	Next = NULL;
}

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
AI::AI()
{
	UserInfo.cannon = false;
	UserInfo.entryFlag = false;
	UserInfo.death = 0;
	UserInfo.kill = 0;
	UserInfo.pos = VECTOR3(0,0,0);
	UserInfo.rot = VECTOR3(0,0,0);
	TargetPos = VECTOR3(0,0,0);
	TargetId = -1;
	DestRotY = 0;
	frame = 0;
	_ReloadTimer = 0;
	LinkList();
}
//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
AI::~AI()
{
	UnlinkList();
}
//------------------------------------------------------------------------------
//初期化
//------------------------------------------------------------------------------
void AI::Initialize(void)
{
	for (int cnt = 0;cnt < UserMax;cnt++)
	{
		AI* ai = new AI;
		ai->ID = cnt;
	}
}
//------------------------------------------------------------------------------
//終了処理
//------------------------------------------------------------------------------
void AI::Finalize(void)
{
	AI* ai = Top;
	AI* next = nullptr;
	while (ai)
	{
		next = ai->Next;
		delete ai;
		ai = nullptr;
		ai = next;
	}
}
//------------------------------------------------------------------------------
//全体更新
//------------------------------------------------------------------------------
void AI::UpdateAll(void)
{
	AI* ai = Top;
	
	//全部の情報を受取る

	while (ai)
	{
		ai->Update();
		ai = ai->Next;
	}

	//全部の情報を送る
}
//------------------------------------------------------------------------------
//更新
//------------------------------------------------------------------------------
void AI::Update(void)
{
	if (UserInfo.entryFlag)
	{
		return ;
	}

	if (frame % 180 == 0)
	{
		SurchTarget();
	}

	UserInfo.pos += Movement;

	if (TargetId)
	{
		VECTOR2 distance = VECTOR2(0,0);
		distance.x = TargetPos.x - UserInfo.pos.x;
		distance.y = TargetPos.z - UserInfo.pos.z;
		DestRotY = atan2(distance.x,distance.y);
	}
	MazzleRevision();

	_ReloadTimer++;

	if (_ReloadTimer >= PLAYER_RELOAD_TIME)
	{
		LaunchFlag = false;
		_ReloadTimer = PLAYER_RELOAD_TIME;
	}
	
	Shot();

	float SubRotY = DestRotY-UserInfo.rot.y;
	REVISE_PI(SubRotY);
	if (SubRotY < -TURN_SPEED)
	{
		SubRotY = -TURN_SPEED;
	}
	else if (SubRotY > TURN_SPEED)
	{
		SubRotY = TURN_SPEED;
	}
	UserInfo.rot.y += SubRotY;

	frame++;
}

//------------------------------------------------------------------------------
//敵を探す
//------------------------------------------------------------------------------
void AI::SurchTarget(void)
{
	double distance = DBL_MAX;
	double disBuff = DBL_MAX;
	int target = -1;
	VECTOR2 Sub = VECTOR2(0,0);
	AI* ai = Top;
	while (ai)
	{
		if (ai->ID == ID)
		{
			continue;
		}
		Sub.x = ai->UserInfo.pos.x - UserInfo.pos.x;
		Sub.y = ai->UserInfo.pos.z - UserInfo.pos.z;
		disBuff = (Sub.x*Sub.x+Sub.y*Sub.y);
		if (disBuff > SURCH_MAX)
		{
			continue;
		}
		if (disBuff < distance)
		{
			disBuff = distance;
			target = ai->ID;
			TargetPos = ai->UserInfo.pos;
			TargetSpeed = ai->Movement;
		}
	}
	TargetId = target;
}
//------------------------------------------------------------------------------
//狙撃
//------------------------------------------------------------------------------
void AI::Shot(void)
{
	if (TargetId == -1)
	{
		BarrelRotX = 0;
		return;
	}

	LaunchFlag = false;
	UserInfo.cannon = true;

	
}
//------------------------------------------------------------------------------
//ユーザー情報をセット
//------------------------------------------------------------------------------
void AI::SetUserInfo(USER_INFO* info)
{
	AI* ai = Top;
	int cnt = 0;
	while (ai)
	{
		ai->UserInfo = info[cnt];
		ai = ai->Next;
		cnt++;
	}
}
//------------------------------------------------------------------------------
//銃口補正
//------------------------------------------------------------------------------
void AI::MazzleRevision(void)
{
	if (TargetId >= 0)
	{
		VECTOR3 DestPos = TargetPos;
		VECTOR3 Sub = TargetPos - UserInfo.pos;
		VECTOR3 Dis = VECTOR3(0,0,0);
		float time = Sub.x / 20.0f;
		float distance = sqrt(Sub.x*Sub.x + Sub.z*Sub.z);

		DestPos += TargetSpeed * (distance / 20.0f);

		Sub = DestPos - UserInfo.pos;
		distance = sqrt(Sub.x*Sub.x + Sub.z*Sub.z);

		Dis.x = 20.0f;
		BarrelRotX = -asin((distance*0.25f) / (20.0f*20.0f))*0.5f;

		DestRotY = atan2(Sub.x,Sub.z);
	}
	REVISE_PI(BarrelRotX);
	REVISE_PI(DestRotY);

	//BallelRotXを送る
}