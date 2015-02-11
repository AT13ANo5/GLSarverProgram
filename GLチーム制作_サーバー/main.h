//=============================================================================
//	main�t�@�C�� [ main.h ]
//	Auther : KOTARO NAGASAKI
//=============================================================================
#ifndef _NET_DATA_H_
#define _NET_DATA_H_

//*****************************************************************************
//	�C���N���[�h
//*****************************************************************************
#include <winsock2.h>
#include "CommonGL.h"
//*****************************************************************************
//	�񋓑̒�`
//*****************************************************************************

//	�f�[�^���e���ʐ����l
//----------------------------------------------------------------
typedef enum
{

	DATA_TYPE_POS = 100,	//	�ʒu
	DATA_TYPE_ROT,	//	��]
	DATA_TYPE_CANNONROT,	//	��]
	DATA_TYPE_CANNON,	//	����
	DATA_TYPE_ENTRY,	//	�G���g���[
	DATA_TYPE_GET_ENTRY,
	DATA_TYPE_CHANGE_GAME,
	DATA_TYPE_CHANGE_RESULT,
	DATA_TYPE_GAME_START,	//	�Q�[���X�^�[�g
	DATA_TYPE_DEATH,	//	����
	DATA_TYPE_KILL,	//	�E����
	DATA_TYPE_PAUSE,	//	�|�[�Y���
	DATA_TYPE_EMPTY,	//	���t
	DATA_TYPE_MAX	//	�ő�l
}DATA_TYPE;
//----------------------------------------------------------------

//*****************************************************************************
//	�\���̒�`
//*****************************************************************************

//	�ʒu���
//----------------------------------------------------------------
typedef struct
{
	float posX;
	float posY;
	float posZ;

}DATA_POS;
//----------------------------------------------------------------

//	��]���
//----------------------------------------------------------------
typedef struct
{
	float rotX;
	float rotY;
	float rotZ;

}DATA_ROT;
//----------------------------------------------------------------

//	���˃t���O
//----------------------------------------------------------------
typedef struct
{
	bool flag;

}DATA_CANNON;
//----------------------------------------------------------------

//	�|�[�Y���p�f�[�^
//----------------------------------------------------------------
typedef struct
{
	int kill;	//	�w��̃v���C���[�̎E���������Z�b�g
	int death;	//	�w��̃v���C���[�̎E���ꂽ�����Z�b�g

}DATA_PAUSE;
//----------------------------------------------------------------

//	�R�l�N�V�����p�f�[�^
//----------------------------------------------------------------
typedef struct
{
	bool entryFlag[6];	//	�G���g���[���Ă邩�ǂ���
}DATA_CONNECTION;
//----------------------------------------------------------------

//	kill death�p�f�[�^
//----------------------------------------------------------------
typedef struct
{
	int value;
}DATA_KILLDEATH;
//----------------------------------------------------------------

//	���M���p�f�[�^
//----------------------------------------------------------------
typedef struct
{
	int ID;
	int type;
	int charNum;
	int servID;

	union
	{
		DATA_POS data_pos;
		DATA_ROT data_rot;
		DATA_ROT data_cannonRot;
		DATA_CANNON data_cannon;
		DATA_PAUSE data_pause;
		DATA_CONNECTION data_connection;
		DATA_KILLDEATH data_killDeath;
	};

}NET_DATA;
//----------------------------------------------------------------

// ���[�U�[���
//----------------------------------------------------------------
typedef struct {
	sockaddr_in fromaddr;	// �A�h���X
	bool entryFlag;	//	�G���g���[�t���O
	int kill;	//	�E������
	int death;	//	�E���ꂽ��

	VECTOR3 pos;
	VECTOR3 rot;
	VECTOR3 cannonRot;
	bool cannon;

} USER_INFO;
//----------------------------------------------------------------

// ����
//----------------------------------------------------------------
typedef struct {
	VECTOR3	position;
	VECTOR3	rotation;
	VECTOR3	scaling;
} ROCK_DATA;
//----------------------------------------------------------------

static const int SERV_ID = 256;

void aiSetPos(int _charNum,VECTOR3 _pos);
void aiSetRot(int _charNum,VECTOR3 _rot);
void aiSetCannonRot(int _charNum,VECTOR3 _cannonRot);
void aiSetCannon(int _charNum,bool _flag);

void SetUserInfomation(int id,USER_INFO info);
VECTOR3 GetRockPos(int index);

//----------------------------------------------------------------
// �����蔻��
//----------------------------------------------------------------
void PushBackCharacter(void);					// �L�����N�^�[���m�̉����߂�
void PushBackRock(void);						// �L�����N�^�[�Ɗ�̉����߂�
void PushBackField(void);						// �n�`�̉����߂�
void PushBackObjectByField(VECTOR3* pPosition);	// �I�u�W�F�N�g�̒n�`�ɂ�鉟���߂�
void PushBackBattleArea(void);					// �s���\�͈͂̉����߂�
bool NeedsSkipPlayer(int index);				// �v���C���[����X�L�b�v
bool NeedsSkipBullet(int index);				// �C�e����X�L�b�v

#endif

//	EOF