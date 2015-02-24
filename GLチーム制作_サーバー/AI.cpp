//=============================================================================
//	AI�N���X
//=============================================================================
//------------------------------------------------------------------------------
//�C���N���[�h
//------------------------------------------------------------------------------
#include "AI.h"
#include "MeshGround.h"
#include<float.h>
#include <math.h>
#include <time.h>

//------------------------------------------------------------------------------
//�}�N����`
//------------------------------------------------------------------------------
// �p�֘A
#define PI     (3.141592654f) // �~����
#define RAD_TO_DEG   (57.29577951f) // 1���W�A�����x
#define DEG_TO_RAD   (0.017453293f) // 1�x�����W�A��

// �x�����W�A��
#define DEG2RAD(degree)  ((degree) * DEG_TO_RAD)
// ���W�A�����x
#define RAD2DEG(radian)  ((radian) * RAD_TO_DEG)

// �p�x�̐��K��(PI�`-PI)
#define REVISE_PI(radian) {if((radian) > PI){(radian) -= (2.0f * PI);}\
								else if((radian) < -PI){(radian) += (2.0f * PI);}}
// �p�x�̐��K��(PI�`-PI)
#define REVISE_PI_DEG(radian) {if((radian) > 180.0f){(radian) -= (360.0f);}\
																else if((radian) < -180.0f){(radian) += 360.0f;}}
//------------------------------------------------------------------------------
//�萔��`
//------------------------------------------------------------------------------
const float PLAYER_MOVE_SPEED (0.05f);			// �ړ��ɂ�����W��
const float TURN_SPEED = 1.0f;		//��]���x
const int PLAYER_RELOAD_TIME (400);			// �����[�h�܂ł̃t���[��
const int UserMax = 6;//�ő僆�[�U�[��
const double SURCH_MAX = 300.0f;	//�t�B�[���h�̃T�C�Y��1/4
const float GRAVITY = 0.06f;				//�e�ɂ�����d�͉����x
const float BULLET_SPEED = 3.5f;			//�e�̑��x
const float FIELD_PANEL_SIZE = 35.0f;			//�t�B�[���h�̃p�l���ꖇ�̃T�C�Y
const float MAXANGLE = (-45.0f);
const float MAX_RANGE = 202.0f;
const float MIN_RANGE = 70.0f;
//------------------------------------------------------------------------------
//�ÓI�����o�ϐ���`
//------------------------------------------------------------------------------
AI* AI::Top = nullptr;
AI* AI::Cur = nullptr;
CMeshGround* AI::Field = nullptr;

float leap(float start,float end,float delta)
{
	return (start*(1.0f-delta) + end*delta);
}
//------------------------------------------------------------------------------
//���X�g�ɒǉ�
//------------------------------------------------------------------------------
void AI::LinkList(void)
{
	if (Top != NULL)//��ڈȍ~�̏���
	{
		AI* ai = Cur;
		ai->Next = this;
		Prev = ai;
		Next = NULL;
		Cur = this;
	}
	else//�ŏ��̈�̎��̏���
	{
		Top = this;
		Cur = this;
		Prev = NULL;
		Next = NULL;
	}
}

//------------------------------------------------------------------------------
//���X�g����폜
//------------------------------------------------------------------------------
void AI::UnlinkList(void)
{
	if (Prev == NULL)//�擪
	{
		if (Next != NULL)//��������
		{
			Next->Prev = NULL;
			Top = Next;
		}
		else//�Ō�̈������
		{
			Top = NULL;
			Cur = NULL;
		}
	}
	else if (Next == NULL)//�I�[
	{
		if (Prev != NULL)//�O������
		{
			Prev->Next = NULL;
			Cur = Prev;
		}
		else//�Ō�̈������
		{
			Top = NULL;
			Cur = NULL;
		}
	}
	else//�O��Ƀf�[�^������Ƃ�
	{
		Prev->Next = Next;
		Next->Prev = Prev;
	}

	Prev = NULL;
	Next = NULL;
}

//------------------------------------------------------------------------------
//�R���X�g���N�^
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
//�f�X�g���N�^
//------------------------------------------------------------------------------
AI::~AI()
{
	UnlinkList();
}
//------------------------------------------------------------------------------
//������
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
//�I������
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
//�S�̍X�V
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
	//�S���̏��𑗂�
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
//�X�V
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
//�G��T��
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
//�_��
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
//���[�U�[�����Z�b�g
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
//�e���␳
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