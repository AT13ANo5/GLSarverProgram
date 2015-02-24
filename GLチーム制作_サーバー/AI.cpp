//=============================================================================
//	AIクラス
//=============================================================================
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "AI.h"
#include "MeshGround.h"
#include<float.h>
#include <math.h>
#include <time.h>

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
// 角度の正規化(PI～-PI)
#define REVISE_PI_DEG(radian) {if((radian) > 180.0f){(radian) -= (360.0f);}\
																else if((radian) < -180.0f){(radian) += 360.0f;}}
//------------------------------------------------------------------------------
//定数定義
//------------------------------------------------------------------------------
const float PLAYER_MOVE_SPEED (0.05f);			// 移動にかかる係数
const float TURN_SPEED = 1.0f;		//回転速度
const int PLAYER_RELOAD_TIME (400);			// リロードまでのフレーム
const int UserMax = 6;//最大ユーザー数
const double SURCH_MAX = 300.0f;	//フィールドのサイズの1/4
const float GRAVITY = 0.06f;				//弾にかかる重力加速度
const float BULLET_SPEED = 3.5f;			//弾の速度
const float FIELD_PANEL_SIZE = 35.0f;			//フィールドのパネル一枚のサイズ
const float MAXANGLE = (-45.0f);
const float MAX_RANGE = 202.0f;
const float MIN_RANGE = 70.0f;
//------------------------------------------------------------------------------
//静的メンバ変数定義
//------------------------------------------------------------------------------
AI* AI::Top = nullptr;
AI* AI::Cur = nullptr;
CMeshGround* AI::Field = nullptr;

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
	UserInfo.pos = VECTOR3(rand() % 2000 - 1000.0f,0,rand() % 2000 - 1000.0f);
	UserInfo.rot = VECTOR3(0,0,0);
	TargetPos = VECTOR3(0,0,0);
	TargetId = -1;
	Speed = PLAYER_MOVE_SPEED;
	DestRotY = 0;
	frame = 0;
	_ReloadTimer = 0;
	Distance = FLT_MAX;
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
void AI::Initialize(int _charNum)
{
	//srand((unsigned)time(nullptr));
	Field = CMeshGround::Create(VECTOR3(0.0f,0.0f,0.0f),VECTOR2(FIELD_PANEL_SIZE,FIELD_PANEL_SIZE),VECTOR2(0,0),1.5f);
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
	Field->Uninit();
	Field = nullptr;
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

	while (ai)
	{
		ai->Update();
		ai = ai->Next;
	}
}

void AI::UpdateInfomation(void)
{
	AI* ai = Top;
	while (ai)
	{
		SetUserInfomation(ai->ID,ai->UserInfo);
		ai = ai->Next;
	}
}

void AI::SendState(void)
{
	AI* ai = Top;
	//全部の情報を送る
	while (ai)
	{
		if (!ai->UserInfo.entryFlag)
		{
			aiSetPos(ai->ID,ai->UserInfo.pos);
			aiSetRot(ai->ID,ai->UserInfo.rot);
			aiSetCannonRot(ai->ID,ai->UserInfo.cannonRot);
			aiSetCannon(ai->ID,ai->UserInfo.cannon);
		}
		ai = ai->Next;
	}
}
//------------------------------------------------------------------------------
//更新
//------------------------------------------------------------------------------
void AI::Update(void)
{
	if (UserInfo.deathFlag)
	{
		UserInfo.cannon = false;
		return ;
	}
	if (UserInfo.entryFlag)
	{
		return ;
	}

	if (frame % 120 == 0)
	{
		if (TargetId < 0)
		{
			SurchTarget();
			
		}
		else
		{
			TargetId = -1;
		}
	}
	printf("ID:%d,%d\n",ID,TargetId);
	UserInfo.pos += Movement;

	MazzleRevision();
	if (TargetId >= 0)
	{
		VECTOR2 distance = VECTOR2(0,0);
		distance.x = TargetPos.x - UserInfo.pos.x;
		distance.y = TargetPos.z - UserInfo.pos.z;
		double dis = sqrt(distance.x*distance.x + distance.y*distance.y);
		if (dis < MIN_RANGE)
		{
			Movement.x -= sinf(DEG2RAD(UserInfo.rot.y)) * Speed;
			Movement.z -= cosf(DEG2RAD(UserInfo.rot.y)) * Speed;
		}
		else if (dis > MAX_RANGE*0.5f)
		{
			Movement.x += sinf(DEG2RAD(UserInfo.rot.y)) * Speed;
			Movement.z += cosf(DEG2RAD(UserInfo.rot.y)) * Speed;
		}
	}
	else
	{
		if (frame % 180 == 0)
		{
			DestRotY = rand()%360 + .0f;
			REVISE_PI_DEG(DestRotY);
			
		}
		Movement.x += sinf(DEG2RAD(UserInfo.rot.y)) * Speed;
		Movement.z += cosf(DEG2RAD(UserInfo.rot.y)) * Speed;
		//SurchTarget();
	}

	frame++;
	if (frame > 60 * 30)
	{
		frame = 0;
	}

	Movement *= 0.95f;
	
	Shot();
	

	float SubRotY = DestRotY-UserInfo.rot.y;
	REVISE_PI_DEG(SubRotY);
	if (SubRotY < -TURN_SPEED)
	{
		SubRotY = -TURN_SPEED;
	}
	else if (SubRotY > TURN_SPEED)
	{
		SubRotY = TURN_SPEED;
	}
	UserInfo.rot.y +=SubRotY;
	REVISE_PI_DEG(UserInfo.rot.y);
	
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
			ai = ai->Next;
			continue;
		}
		Sub.x = ai->UserInfo.pos.x - UserInfo.pos.x;
		Sub.y = ai->UserInfo.pos.z - UserInfo.pos.z;
		disBuff = sqrt(Sub.x*Sub.x+Sub.y*Sub.y);
		if (disBuff > SURCH_MAX)
		{
			ai = ai->Next;
			continue;
		}
		if (disBuff < distance)
		{
			disBuff = distance;
			target = ai->ID;
			TargetPos = ai->UserInfo.pos;
			TargetSpeed = ai->Movement;
		}
		ai = ai->Next;
	}
	TargetId = target;
}
//------------------------------------------------------------------------------
//狙撃
//------------------------------------------------------------------------------
void AI::Shot(void)
{
	if (LaunchFlag == false)
	{
		if (TargetId == -1 || Distance > MAX_RANGE)
		{
			BarrelRotX = 0;
			return;
		}
		UserInfo.cannon = true;
		LaunchFlag = true;
		_ReloadTimer = 0;
	}
	else
	{
		_ReloadTimer++;
		UserInfo.cannon = false;
		if (_ReloadTimer >= PLAYER_RELOAD_TIME)
		{
			LaunchFlag = false;
		}
	}
}
//------------------------------------------------------------------------------
//ユーザー情報をセット
//------------------------------------------------------------------------------
void AI::SetUserInfo(USER_INFO* info, int _charNum)
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
		float time = Sub.x / BULLET_SPEED;
		Distance = sqrt(Sub.x*Sub.x + Sub.z*Sub.z);

		DestPos += TargetSpeed * (Distance / BULLET_SPEED);

		Sub = DestPos - UserInfo.pos;
		Distance = sqrt(Sub.x*Sub.x + Sub.z*Sub.z);

		Dis.x = BULLET_SPEED;

		//BarrelRotX = -asin((distance*GRAVITY) / (BULLET_SPEED*BULLET_SPEED))*0.5f;

		float per = Distance/MAX_RANGE;
		if (Distance > MAX_RANGE)
		{
			BarrelRotX = DEG2RAD(-10.0f);
		}
		else
		{
			BarrelRotX = DEG2RAD(MAXANGLE*per);
		}
		DestRotY = atan2(Sub.x,Sub.z);
		REVISE_PI(DestRotY);
		DestRotY = RAD2DEG(DestRotY);
	}
	REVISE_PI(BarrelRotX);

	UserInfo.cannonRot = UserInfo.rot;
	UserInfo.cannonRot.x = RAD2DEG(BarrelRotX);
}