//=============================================================================
//	main�t�@�C�� [ main.cpp ]
//	Auther : KOTARO NAGASAKI
//=============================================================================

//*****************************************************************************
//	�萔��`
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

//*****************************************************************************
//	�C���N���[�h
//*****************************************************************************
#include <ws2tcpip.h>
#include <float.h>
#include <math.h>
#include <time.h>
#include<process.h>
#include "main.h"
#include "AI.h"
#include "MeshGround.h"

//*****************************************************************************
//	���C�u�����̃����N
//*****************************************************************************
#pragma comment ( lib , "ws2_32.lib" )
#pragma comment (lib, "winmm.lib")

//*****************************************************************************
//	�v���g�^�C�v�錾
//*****************************************************************************
unsigned __stdcall aiUpdate(void *p);

//*****************************************************************************
//	�萔��`
//*****************************************************************************
const int charcterMax = 6;
const int hostNameMax = 256;
const int sendPort = 20000;
USER_INFO userInfo[charcterMax];
SOCKET sendSock;
sockaddr_in sendAdd;
bool gameStartFlag;

bool threadEndFlag = false;

int timer;
int gameTime;
HANDLE ai;

const float	RADIUS_DEFENSE_CHARACTER = 12.0f;	// �L�����N�^�[�̖h�䔼�a
const float	HEIGHT_DEFENSE_CHARACTER = 0.0f;	// �L�����N�^�[�̖h�䒆�S����
const float	RADIUS_OFFENSE_BULLET = 10.0f;		// �C�e�̍U�����a
const float	HEIGHT_OFFENSE_BULLET = 0.0f;		// �C�e�̍U�����S����
const float	RADIUS_PUSH_CHARACTER = 10.0f;		// �L�����N�^�[�̉����߂����a
const float	HEIGHT_PUSH_CHARACTER = 0.0f;		// �L�����N�^�[�̉����߂����S����
const float	RADIUS_DEFENSE_ROCK = 28.0f;		// ��̖h�䔼�a
const float	HEIGHT_DEFENSE_ROCK = 0.0f;			// ��̖h�䒆�S����
const float	RADIUS_PUSH_ROCK = 38.0f;			// ��̉����߂����a
const float	HEIGHT_PUSH_ROCK = 0.0f;			// ��̉����߂����S����
const float FIELD_PANEL_SIZE = 35.0f;			//�t�B�[���h�̃p�l���ꖇ�̃T�C�Y
const float	HEIGHT_PLAYER_TO_FIELD = 10.0f;		// �v���C���[�ƒn�ʂ̍�
const float	RADIUS_AREA_BATTLE = 1000.0f;		// �퓬�G���A���a

const ROCK_DATA ROCK_DATA_LIST[] = {
	{ VECTOR3(-214.0f,100.0f,421.0f),	VECTOR3( 0.0f, 0.0f, 0.0f ),	VECTOR3( 1.0f, 1.0f, 1.0f ) },
	{ VECTOR3(359.0f,100.0f,188.0f),	VECTOR3( 0.0f, 0.0f, 0.0f ),	VECTOR3( 1.0f, 1.0f, 1.0f ) },
	{ VECTOR3(94.0f,100.0f,-458.0f),	VECTOR3( 0.0f, 0.0f, 0.0f ),	VECTOR3( 1.0f, 1.0f, 1.0f ) },
	{ VECTOR3(-198.0f,100.0f,222.0f),	VECTOR3( 0.0f, 0.0f, 0.0f ),	VECTOR3( 1.0f, 1.0f, 1.0f ) },
	{ VECTOR3(419.0f,100.0f,293.0f),	VECTOR3( 0.0f, 0.0f, 0.0f ),	VECTOR3( 1.0f, 1.0f, 1.0f ) },
	{ VECTOR3(-325.0f,100.0f,164.0f),	VECTOR3( 0.0f, 0.0f, 0.0f ),	VECTOR3( 1.0f, 1.0f, 1.0f ) },
	{ VECTOR3(-471.0f,100.0f,-115.0f),	VECTOR3( 0.0f, 0.0f, 0.0f ),	VECTOR3( 1.0f, 1.0f, 1.0f ) },
	{ VECTOR3(368.0f,100.0f,-373.0f),	VECTOR3( 0.0f, 0.0f, 0.0f ),	VECTOR3( 1.0f, 1.0f, 1.0f ) },
	{ VECTOR3(-476.0f,100.0f,231.0f),	VECTOR3( 0.0f, 0.0f, 0.0f ),	VECTOR3( 1.0f, 1.0f, 1.0f ) },
	{ VECTOR3(-249.0f,100.0f,-319.0f),	VECTOR3( 0.0f, 0.0f, 0.0f ),	VECTOR3( 1.0f, 1.0f, 1.0f ) },
	{ VECTOR3(-243.0f,100.0f,481.0f),	VECTOR3( 0.0f, 0.0f, 0.0f ),	VECTOR3( 1.0f, 1.0f, 1.0f ) },
	{ VECTOR3(345.0f,100.0f,-253.0f),	VECTOR3( 0.0f, 0.0f, 0.0f ),	VECTOR3( 1.0f, 1.0f, 1.0f ) },
	{ VECTOR3(444.0f,100.0f,-118.0f),	VECTOR3( 0.0f, 0.0f, 0.0f ),	VECTOR3( 1.0f, 1.0f, 1.0f ) },
	{ VECTOR3(186.0f,100.0f,27.0f),		VECTOR3( 0.0f, 0.0f, 0.0f ),	VECTOR3( 1.0f, 1.0f, 1.0f ) },
	{ VECTOR3(387.0f,100.0f,-438.0f),	VECTOR3( 0.0f, 0.0f, 0.0f ),	VECTOR3( 1.0f, 1.0f, 1.0f ) },
	{ VECTOR3(-12.0f,100.0f,-439.0f),	VECTOR3( 0.0f, 0.0f, 0.0f ),	VECTOR3( 1.0f, 1.0f, 1.0f ) },
	{ VECTOR3(496.0f,100.0f,-332.0f),	VECTOR3( 0.0f, 0.0f, 0.0f ),	VECTOR3( 1.0f, 1.0f, 1.0f ) },
	{ VECTOR3(-247.0f,100.0f,143.0f),	VECTOR3( 0.0f, 0.0f, 0.0f ),	VECTOR3( 1.0f, 1.0f, 1.0f ) },
	{ VECTOR3(-302.0f,100.0f,-296.0f),	VECTOR3( 0.0f, 0.0f, 0.0f ),	VECTOR3( 1.0f, 1.0f, 1.0f ) },
	{ VECTOR3(-171.0f,100.0f,-274.0f),	VECTOR3( 0.0f, 0.0f, 0.0f ),	VECTOR3( 1.0f, 1.0f, 1.0f ) }
};

const VECTOR3 PLAYER_POSITION_LIST[] = {
	VECTOR3(-0.0f, 100.0f, 800),
	VECTOR3(690, 100.0f, 400),
	VECTOR3(690, 100.0f, -400),
	VECTOR3(0, 100.0f, -800),
	VECTOR3(-700, 100.0f, -400),
	VECTOR3(-700, 100.0f, 400),
};

const VECTOR3 PLAYER_ROTATION_LIST[] = {
	VECTOR3(0, 180, 0),
	VECTOR3(0, 240.0f, 0),
	VECTOR3(0, 300.0f, 0),
	VECTOR3(0, 0, 0),
	VECTOR3(0, 60.0f, 0),
	VECTOR3(0, 120.0f, 0),
};

CMeshGround*	Ground;		// �t�B�[���h

//	���݂̃L�����N�^�[��
int charNum = -1;

//=============================================================================
//	����o�C���h�֐�
//=============================================================================
bool myBind(SOCKET* _socket, SOCKADDR_IN* _sockAdd)
{
	int ret;

	for (;;)
	{
		int portAdd = 1;

		ret = bind(*_socket, (sockaddr*)_sockAdd, sizeof(*_sockAdd));
		ret = WSAGetLastError();

		if (ret == WSAEADDRINUSE)
		{
			_sockAdd->sin_port = htons(20000 + portAdd);
			portAdd++;
			continue;
		}
		else
			break;
	}

	return true;
}

//=============================================================================
//	���[�U�[��񃊃Z�b�g����
//=============================================================================
void initUserInfo()
{
	for (int count = 0; count < charcterMax; count++)
	{
		memset(&userInfo[count].fromaddr, 0, sizeof(userInfo[count].fromaddr));
		userInfo[count].entryFlag = false;
		userInfo[count].death = 0;
		userInfo[count].kill = 0;
		userInfo[count].pos = PLAYER_POSITION_LIST[count];
		userInfo[count].rot = PLAYER_ROTATION_LIST[count];
		userInfo[count].cannonRot = VECTOR3(0.0f, 0.0f, 0.0f);
		userInfo[count].cannon = false;
		userInfo[count].deathFlag = false;
	}
}
//=============================================================================
//	�^�C�}�[���M����
//=============================================================================
void sendTimer()
{
	NET_DATA data;

	data.type = DATA_TYPE_TIMER;
	data.servID = SERV_ID;

	//	�}���`�L���X�g�ő��M�i���M��Ŏ����̃f�[�^�������珟��ɂ͂����j
	sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));
}
//=============================================================================
//	���U���g�ւ̈ڍs���M����
//=============================================================================
void sendChangeResult()
{
	NET_DATA data;

	data.type = DATA_TYPE_CHANGE_RESULT;
	data.charNum = 0;
	data.servID = SERV_ID;

	//	�}���`�L���X�g�ő��M�i���M��Ŏ����̃f�[�^�������珟��ɂ͂����j
	sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));
}
//=============================================================================
//	ai�p�ʒu�Z�b�g�����M����
//=============================================================================
void aiSetPos(int _charNum, VECTOR3 _pos)
{
	NET_DATA data;

	//	�ʒu���̃Z�b�g
	userInfo[_charNum].pos.x = _pos.x;
	userInfo[_charNum].pos.y = _pos.y;
	userInfo[_charNum].pos.z = _pos.z;

	data.charNum = _charNum;
	data.type = DATA_TYPE_POS;
	data.servID = SERV_ID;
	data.data_pos.posX = _pos.x;
	data.data_pos.posY = _pos.y;
	data.data_pos.posZ = _pos.z;

	//	�}���`�L���X�g�ő��M�i���M��Ŏ����̃f�[�^�������珟��ɂ͂����j
	sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));
}

//=============================================================================
//	ai�p�ʒu�Z�b�g�����M����
//=============================================================================
void aiSetRot(int _charNum, VECTOR3 _rot)
{
	NET_DATA data;

	//	�ʒu���̃Z�b�g
	userInfo[_charNum].rot.x = _rot.x;
	userInfo[_charNum].rot.y = _rot.y;
	userInfo[_charNum].rot.z = _rot.z;

	data.charNum = _charNum;
	data.type = DATA_TYPE_ROT;
	data.servID = SERV_ID;
	/*data.data_rot.rotX = _rot.x;
	data.data_rot.rotY = _rot.y;
	data.data_rot.rotZ = _rot.z;*/

	data.data_rot.rotY = _rot.y;

	//	�}���`�L���X�g�ő��M�i���M��Ŏ����̃f�[�^�������珟��ɂ͂����j
	sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));
}

//=============================================================================
//	ai�p�ʒu�Z�b�g�����M����
//=============================================================================
void aiSetCannonRot(int _charNum, VECTOR3 _cannonRot)
{
	NET_DATA data;

	//	�ʒu���̃Z�b�g
	userInfo[_charNum].cannonRot.x = _cannonRot.x;
	userInfo[_charNum].cannonRot.y = _cannonRot.y;
	userInfo[_charNum].cannonRot.z = _cannonRot.z;

	data.charNum = _charNum;
	data.type = DATA_TYPE_CANNONROT;
	data.servID = SERV_ID;
	data.data_cannonRot.rotX = _cannonRot.x;
	data.data_cannonRot.rotY = _cannonRot.y;
	data.data_cannonRot.rotZ = _cannonRot.z;

	//	�}���`�L���X�g�ő��M�i���M��Ŏ����̃f�[�^�������珟��ɂ͂����j
	sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));
}

//=============================================================================
//	ai�p�ʒu�Z�b�g�����M����
//=============================================================================
void aiSetCannon(int _charNum, bool _flag)
{
	NET_DATA data;

	//	�ʒu���̃Z�b�g
	userInfo[_charNum].cannon = _flag;

	data.charNum = _charNum;
	data.type = DATA_TYPE_CANNON;
	data.servID = SERV_ID;
	data.data_cannon.flag = _flag;

	//	�}���`�L���X�g�ő��M�i���M��Ŏ����̃f�[�^�������珟��ɂ͂����j
	sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));
}
//=============================================================================
//	���Z�b�g�����֐�
//=============================================================================
void Reset()
{
	initUserInfo();

	charNum = -1;

	timer = 0;
	gameTime = 183;
	ai = 0;

	gameStartFlag = false;
}
//=============================================================================
//	���C�������֐�
//=============================================================================
int main(void)
{
	WSADATA wsaData;

	// �z�X�g���EIP�A�h���X
	char hostName[hostNameMax];
	char ip[16];
	PHOSTENT phostent;
	IN_ADDR in;

	gameStartFlag = false;

	charNum = -1;

	//	Winsock�̏�����
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NULL)
	{
		printf("Winsock�̏��������s\n�G���[�R�[�h : %d\n", WSAGetLastError());

		return false;
	}

	//	�����̃z�X�g���擾
	gethostname(hostName, sizeof(hostName));
	phostent = gethostbyname(hostName);

	//	������IP�A�h���X��\���i���[�U�[�͕\������Ă���IP�A�h���X��"address.txt"�ɃZ�b�g����j
	memcpy(&in, phostent->h_addr, 4);
	sprintf_s(ip, inet_ntoa(in));
	printf("%s:%s\n", hostName, ip);

	//	���M���p�ϐ��Q����
	//-------------------------------------------------
	//	�\�P�b�g�̐���
	sendSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//	���M��A�h���X
	sendAdd.sin_port = htons(2000);
	sendAdd.sin_family = AF_INET;
	sendAdd.sin_addr.s_addr = inet_addr("239.0.0.23");//�}���`�L���X�g�A�h���X

	int param = 1;
	int ret = setsockopt(sendSock, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&param, sizeof(param));
	ret = WSAGetLastError();
	//-------------------------------------------------

	//	��M���p�ϐ��Q����
	//-------------------------------------------------
	//	�\�P�b�g�̐���
	SOCKET recvSock;
	recvSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//	��M��̃A�h���X
	sockaddr_in recvAdd;
	recvAdd.sin_port = htons(3000);
	recvAdd.sin_family = AF_INET;
	recvAdd.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	int recvAddLength = sizeof(recvAdd);

	setsockopt(recvSock, SOL_SOCKET, SO_BROADCAST, (char *)&param, sizeof(param));

	myBind(&recvSock, &recvAdd);
	//-------------------------------------------------

	initUserInfo();


	timer = 0;
	gameTime = 183;


	UINT threadID = 0;

	ai = 0;

#ifdef _DEBUG
	//ai = (HANDLE)_beginthreadex(NULL, 0, &aiUpdate, NULL, 0, &threadID);
#endif
	NET_DATA data;

	// �n�`����
	Ground = CMeshGround::Create(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR2(FIELD_PANEL_SIZE, FIELD_PANEL_SIZE), VECTOR2(0, 0), 1.5f);

	for (;;)
	{
		//	��M
		int ret = recvfrom(recvSock, (char*)&data, sizeof(data), 0, (sockaddr*)&recvAdd, &recvAddLength);
		//ret = WSAGetLastError();
		//printf("%d", ret);]

		if (ret == SOCKET_ERROR)
		{
		}
		else
		{
			if (data.servID == SERV_ID)
			{
				//	�f�[�^�^�C�v�ɂ���ĕ���
				switch (data.type)
				{
				case DATA_TYPE_POS:

					if (userInfo[data.charNum].entryFlag == true)
					{
						//	�ʒu���̃Z�b�g
						userInfo[data.charNum].pos.x = data.data_pos.posX;
						userInfo[data.charNum].pos.y = data.data_pos.posY;
						userInfo[data.charNum].pos.z = data.data_pos.posZ;

						//	�}���`�L���X�g�ő��M�i���M��Ŏ����̃f�[�^�������珟��ɂ͂����j
						sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));
					}

					if (userInfo[data.charNum].deathFlag == true)
					{
						if (userInfo[data.charNum].entryFlag == false)
						{
							//	�ʒu���̃Z�b�g
							userInfo[data.charNum].pos.x = data.data_pos.posX;
							userInfo[data.charNum].pos.y = data.data_pos.posY;
							userInfo[data.charNum].pos.z = data.data_pos.posZ;

							//	�}���`�L���X�g�ő��M�i���M��Ŏ����̃f�[�^�������珟��ɂ͂����j
							sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));
						}
					}

					break;

				case DATA_TYPE_ROT:

					if (userInfo[data.charNum].entryFlag == true)
					{
						//	��]���̃Z�b�g
						userInfo[data.charNum].rot.y = data.data_rot.rotY;

						//	�}���`�L���X�g�ő��M�i���M��Ŏ����̃f�[�^�������珟��ɂ͂����j
						sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));
					}

					if (userInfo[data.charNum].deathFlag == true)
					{
						if (userInfo[data.charNum].entryFlag == false)
						{
							//	��]���̃Z�b�g
							userInfo[data.charNum].rot.y = data.data_rot.rotY;
						}
					}

					break;

				case DATA_TYPE_CANNONROT:

					if (userInfo[data.charNum].entryFlag == true)
					{
						//	��]���̃Z�b�g
						userInfo[data.charNum].cannonRot.x = data.data_cannonRot.rotX;
						userInfo[data.charNum].cannonRot.y = data.data_cannonRot.rotY;
						userInfo[data.charNum].cannonRot.z = data.data_cannonRot.rotZ;

						//	�}���`�L���X�g�ő��M�i���M��Ŏ����̃f�[�^�������珟��ɂ͂����j
						sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));
					}

					if (userInfo[data.charNum].deathFlag == true)
					{
						if (userInfo[data.charNum].entryFlag == false)
						{
							//	��]���̃Z�b�g
							userInfo[data.charNum].cannonRot.x = data.data_cannonRot.rotX;
							userInfo[data.charNum].cannonRot.y = data.data_cannonRot.rotY;
							userInfo[data.charNum].cannonRot.z = data.data_cannonRot.rotZ;
						}
					}

					break;

				case DATA_TYPE_CANNON:

					if (userInfo[data.charNum].entryFlag == true)
					{
						//	���˃t���O�̃Z�b�g
						userInfo[data.charNum].cannon = data.data_cannon.flag;

						//	�}���`�L���X�g�ő��M�i���M��Ŏ����̃f�[�^�������珟��ɂ͂����j
						sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));
					}

					break;

				case DATA_TYPE_KILL:

					//	�E�������C���N�������g
					userInfo[data.charNum].kill = data.data_killDeath.value;

					//	�}���`�L���X�g�ő��M�i���M��Ŏ����̃f�[�^�������珟��ɂ͂����j
					sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));

					break;

				case DATA_TYPE_DEATH:

					//	�E���ꂽ���C���N�������g
					userInfo[data.charNum].death = data.data_killDeath.value;

					//	�}���`�L���X�g�ő��M�i���M��Ŏ����̃f�[�^�������珟��ɂ͂����j
					sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));

					break;


				case DATA_TYPE_SEND_DEATH:

					userInfo[data.charNum].deathFlag = true;

					break;

				case DATA_TYPE_SEND_REBORN:

					userInfo[data.charNum].deathFlag = false;

					break;

				case DATA_TYPE_PAUSE:

					//	�|�[�Y���Ƀf�[�^��n��
					//	�w��̃L�����N�^�̂Ƃ���ɂ̂ݑ��M
					for (int count = 0; count < charcterMax; count++)
					{
						//	�����ȊO�̏ꍇ
						if (count != data.charNum)
						{
							data.charNum = count;

							//	���M�f�[�^�i�[
							data.servID = SERV_ID;
							data.type = DATA_TYPE_PAUSE;
							data.data_pause.kill = userInfo[count].kill;
							data.data_pause.death = userInfo[count].death;

							//	�}���`�L���X�g�ő��M�i���M��Ŏ����̃f�[�^�������珟��ɂ͂����j
							sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));
						}
					}

					break;

				case DATA_TYPE_GAME_START:

					//	�ŏ�ʃN���C�A���g����A�Q�[���J�n���󂯎������
					if (data.charNum == 0)
					{
						AI::Initialize(charNum + 1);

						// AI�����p�X���b�h�J�n
						if (ai == 0)
						{
							ai = (HANDLE)_beginthreadex(NULL,0,&aiUpdate,NULL,NULL,NULL);
						}

						for (int count = charNum + 1; count < charcterMax; count++)
						{
							//	���[�U�[�����Z�b�g
							userInfo[count].fromaddr = recvAdd;
							userInfo[count].fromaddr.sin_port = htons(3000);
							userInfo[count].entryFlag = false;
							userInfo[count].deathFlag = true;
						}

						//	�Q�[���X�^�[�g������
						gameStartFlag = true;
						printf("GameStart!\n");
					}

					break;

				case DATA_TYPE_CHANGE_GAME:

					//	�ŏ�ʃN���C�A���g����A�Q�[���ւ̑J�ڂ��󂯎������
					if (data.charNum == 0)
					{
						//	���̃N���C�A���g�ɂ��Q�[���ւ̑J�ڂ�`����
						//	�}���`�L���X�g�ő��M�i���M��Ŏ����̃f�[�^�������珟��ɂ͂����j
						sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));
					}

					break;

				case DATA_TYPE_CHANGE_RESULT:

					if (data.charNum == 0)
					{
						Reset();
						threadEndFlag = true;
						CloseHandle(ai);
						ai = 0;
					}

					break;

				case DATA_TYPE_GET_ENTRY:

					data.type = DATA_TYPE_GET_ENTRY;
					data.servID = SERV_ID;

					for (int count = 0; count < 6; count++)
					{
						data.data_connection.entryFlag[count] = userInfo[count].entryFlag;
					}

					//	�|�[�g���Đݒ�
					recvAdd.sin_port = htons(3000);

					//	�}���`�L���X�g�ő��M�i���M��Ŏ����̃f�[�^�������珟��ɂ͂����j
					sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));

					break;

				case DATA_TYPE_ENTRY:

					//	�Q�[���X�^�[�g���Ă��Ȃ����
					if (gameStartFlag == false)
					{
						//	�G���g���[����
						//	���݂̃v���C���[����Ԃ��A�����̃v���C���[�ԍ����Z�b�g������B
						{
							//	���[�v��������t���O
							bool errorFlag = true;

							//	�L�����N�^�[�ԍ����C���N�������g�i�L�����N�^�[�ԍ���0�`5�j
							charNum++;

							//	�L�����N�^�[�������l�𒴂��Ă����珈�����͂���
							if (charNum < charcterMax)
							{
								//	�G���[�����i�G���g���[��ԂȂ�G���[�Ƃ���j
								if (userInfo[charNum].entryFlag == true)
								{
									//	�L�����N�^�ԍ����}�C�i�X���A������e��
									charNum--;
									errorFlag = false;

									break;
								}

								//	�G���[�����𔲂�����
								if (errorFlag == true)
								{
									//	���l���Z�b�g
									data.charNum = charNum;
									data.servID = SERV_ID;
									data.type = DATA_TYPE_ENTRY;

									//	���[�U�[�����Z�b�g
									userInfo[charNum].fromaddr = recvAdd;
									userInfo[charNum].fromaddr.sin_port = htons(3000);
									userInfo[charNum].entryFlag = true;

									//	�|�[�g���Đݒ�
									recvAdd.sin_port = htons(3000);

									printf("Player%d Entry\n",charNum+1);

									//	�u�G���g���[�����v�Ƃ��������}���`�L���X�g�ő��M
									sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));
								}
							}
							else
							{
								data.type = DATA_TYPE_EMPTY;
								data.servID = SERV_ID;

								//	�|�[�g���Đݒ�
								recvAdd.sin_port = htons(3000);

								//	�����������ς��Ƒ���
								//	�}���`�L���X�g�ő��M�i���M��Ŏ����̃f�[�^�������珟��ɂ͂����j
								sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));
							}

							break;
						}
					}
				}
			}
			else
			{
				//printf("networkID Not Equal!!\n");
			}
		}
	}

	// �n�`�̔j��
	SafeDelete(Ground);

	CloseHandle(ai);
	ai = 0;
	// �\�P�b�g�I��
	closesocket(recvSock);

	// WinSock�I������
	WSACleanup();

	AI::Finalize();
}

//=============================================================================
//	AI�p�X�V�����֐�
//=============================================================================
unsigned __stdcall aiUpdate(void *p)
{
	unsigned int CurrentTime = 0;//DWORD dwCurrentTime;
	unsigned int PrevTime = 0;	 //DWORD dwExecLastTime;
	unsigned int FPSLastTime = 0;//DWORD dwFPSLastTime;
	unsigned int FrameCount = 0; //DWORD dwFrameCount;

	threadEndFlag = false;

	timeBeginPeriod(1);				// ����\��ݒ�

	FPSLastTime =
		CurrentTime = timeGetTime();

	PrevTime = 0;

	FrameCount = 0;

	if (charNum == 0)
	{
		userInfo[1].entryFlag = false;
	}

	//	�N���C�A���g����FPS�����낦��
	for (;;)
	{
		CurrentTime = timeGetTime();
		if ((CurrentTime - FPSLastTime) >= 500)	// 0.5�b���ƂɎ��s
		{
			FPSLastTime = CurrentTime;
			FrameCount = 0;
		}

		if (CurrentTime - PrevTime >= 1000 / 60)
		{
			//	AI�X�V����
			AI::SetUserInfo(userInfo, charNum + 1);
			AI::UpdateAll();
			AI::UpdateInfomation();
			PushBackCharacter();
			PushBackRock();
			PushBackField();
			PushBackBattleArea();
			for (int cnt = charNum + 1; cnt < charcterMax; cnt++)
			{
				if (userInfo[cnt].entryFlag == false)
				{
					if (userInfo[cnt].deathFlag == false)
					{
						aiSetPos(cnt, userInfo[cnt].pos);
						aiSetRot(cnt, userInfo[cnt].rot);
						aiSetCannonRot(cnt, userInfo[cnt].cannonRot);
						aiSetCannon(cnt, userInfo[cnt].cannon);
					}
				}
			}
			
			PrevTime = CurrentTime;

			timer++;

			if (timer == 60)
			{
				sendTimer();
				timer = 0;
				gameTime--;

				/*if (gameTime == -1)
				{
					//sendChangeResult();
					AI::Finalize();
					CloseHandle(ai);
					ai = 0;
				}*/
			}
		}

		if (threadEndFlag)
		{
			AI::Finalize();
			_endthreadex(0);
			break;
		}
	}

	return 0;
}

VECTOR3 GetRockPos(int index)
{
	if (index >= 0 && index < 20)
	{
		return ROCK_DATA_LIST[index].position;
	}

	return VECTOR3(-1.0f,-1.0f,-1.0f);
}

void SetUserInfomation(int id,USER_INFO info)
{
	if (id >= 0 && id < charcterMax)
	{
		userInfo[id] = info;
	}
}

//==============================================================================
// �L�����N�^�[���m�̉����߂�
//==============================================================================
void PushBackCharacter(void)
{
	// �U�����̌���
	USER_INFO	infoOffense;		// �U�����v���C���[
	for (int cntOffense = 0; cntOffense < charcterMax; ++cntOffense)
	{
		// �v���C���[���擾
		infoOffense = userInfo[cntOffense];

		// �v���C���[������\���m�F
		if (NeedsSkipPlayer(cntOffense))
		{
			continue;
		}

		// �h�䑤�̌���
		for (int cntDefense = 0; cntDefense < charcterMax; ++cntDefense)
		{
			// �v���C���[���擾
			USER_INFO	infoDefense = userInfo[cntDefense];		// �h�䑤�v���C���[

			// �v���C���[������\���m�F
			if (NeedsSkipPlayer(cntDefense))
			{
				continue;
			}
			if (cntOffense == cntDefense)
			{
				continue;
			}

			// �����蔻��
			VECTOR3	positionOffense = infoOffense.pos;		// �U�����蒆�S���W
			VECTOR3	positionDefense = infoDefense.pos;		// �h�䔻�蒆�S���W
			VECTOR3	vectorOffenseToDefense;					// �U�����肩��h�䔻��ւ̃x�N�g��
			float	distanceOffenseAndDefense;				// ����̒��S���m�̋���
			positionOffense.y += HEIGHT_PUSH_CHARACTER;
			positionDefense.y += HEIGHT_PUSH_CHARACTER;
			vectorOffenseToDefense = positionDefense - positionOffense;
			distanceOffenseAndDefense = sqrtf(vectorOffenseToDefense.x * vectorOffenseToDefense.x + vectorOffenseToDefense.y * vectorOffenseToDefense.y + vectorOffenseToDefense.z * vectorOffenseToDefense.z);
			if (distanceOffenseAndDefense < 2.0f * RADIUS_PUSH_CHARACTER)
			{
				// �����߂�
				if (distanceOffenseAndDefense < -FLT_EPSILON || distanceOffenseAndDefense > FLT_EPSILON)
				{
					VECTOR3	vectorPushBack = vectorOffenseToDefense * 0.51f * (2.0f * RADIUS_PUSH_CHARACTER - distanceOffenseAndDefense) / distanceOffenseAndDefense;
					infoDefense.pos += vectorPushBack;
					vectorPushBack *= -1.0f;
					infoOffense.pos += vectorPushBack;
				}
				else
				{
					infoOffense.pos.x += RADIUS_PUSH_CHARACTER;
					infoDefense.pos.x -= RADIUS_PUSH_CHARACTER;
				}

				// �G�t�F�N�g�F�Ήԁ@�v���C���[���m�̂Ԃ���
			}
			userInfo[cntDefense].pos = infoDefense.pos;
		}
		userInfo[cntOffense].pos = infoOffense.pos;
	}
}

//==============================================================================
// �L�����N�^�[�Ɗ�̉����߂�
//==============================================================================
void PushBackRock(void)
{
	// �U�����̌���
	USER_INFO	infoPlayer;		// �U�����v���C���[
	for (int cntPlayer = 0; cntPlayer < charcterMax; ++cntPlayer)
	{
		// �v���C���[���擾
		infoPlayer = userInfo[cntPlayer];

		// �v���C���[������\���m�F
		if (NeedsSkipPlayer(cntPlayer))
		{
			continue;
		}

		// �h�䑤�̌���
		VECTOR3	positioninfoPlayer = infoPlayer.pos;						// �U�����蒆�S���W
		int		maxRockData = sizeof(ROCK_DATA_LIST) / sizeof(ROCK_DATA);	// �ő�␔
		for (int cntRock = 0; cntRock < maxRockData; ++cntRock)
		{
			// �����蔻��
			VECTOR3	positionRock = ROCK_DATA_LIST[cntRock].position;	// �h�䔻�蒆�S���W
			VECTOR3	vectorOffenseToDefense;								// �U�����肩��h�䔻��ւ̃x�N�g��
			float	distanceOffenseAndDefense;							// ����̒��S���m�̋���
			float	scalingRock;										// ��̑傫��
			scalingRock = (ROCK_DATA_LIST[cntRock].scaling.x < ROCK_DATA_LIST[cntRock].scaling.z ? ROCK_DATA_LIST[cntRock].scaling.x : ROCK_DATA_LIST[cntRock].scaling.z);
			positioninfoPlayer.y += HEIGHT_PUSH_CHARACTER;
			positionRock.y += HEIGHT_PUSH_ROCK;
			vectorOffenseToDefense = positionRock - positioninfoPlayer;
			distanceOffenseAndDefense = sqrtf(vectorOffenseToDefense.x * vectorOffenseToDefense.x + vectorOffenseToDefense.y * vectorOffenseToDefense.y + vectorOffenseToDefense.z * vectorOffenseToDefense.z);

			if (distanceOffenseAndDefense < RADIUS_PUSH_CHARACTER + RADIUS_PUSH_ROCK * scalingRock)
			{
				// �����߂�
				if (distanceOffenseAndDefense < -FLT_EPSILON || distanceOffenseAndDefense > FLT_EPSILON)
				{
					VECTOR3	vectorPushBack = vectorOffenseToDefense * -(RADIUS_PUSH_CHARACTER + RADIUS_PUSH_ROCK * scalingRock - distanceOffenseAndDefense) / distanceOffenseAndDefense;
					vectorPushBack.y = 0.0f;
					infoPlayer.pos += vectorPushBack;
				}
				else
				{
					infoPlayer.pos.x += RADIUS_PUSH_CHARACTER + RADIUS_PUSH_ROCK * scalingRock;
				}
			}
		}
		userInfo[cntPlayer].pos = infoPlayer.pos;
	}
}

//==============================================================================
// �n�`�̉����߂�
//==============================================================================
void PushBackField(void)
{
	// ����
	USER_INFO	infoPlayer;		// �ΏۃI�u�W�F�N�g
	for (int cntPlayer = 0; cntPlayer < charcterMax; ++cntPlayer)
	{
		// �ΏۃI�u�W�F�N�g���擾
		infoPlayer = userInfo[cntPlayer];

		// �Ώۂ̃X�e�[�g���m�F
		if (NeedsSkipPlayer(cntPlayer))
		{
			continue;
		}

		// �����߂�
		PushBackObjectByField(&userInfo[cntPlayer].pos);
	}
}

//==============================================================================
// �I�u�W�F�N�g�̒n�`�ɂ�鉟���߂�
//==============================================================================
void PushBackObjectByField(VECTOR3* pPosition)
{
	// �n�`�Ƃ̂����蔻��
	VECTOR3	NormalGround;		// �n�`�̖@��
	pPosition->y = Ground->GetHeight(*pPosition, &NormalGround) + HEIGHT_PLAYER_TO_FIELD;;
}

//==============================================================================
// �퓬�͈͉����߂�
//==============================================================================
void PushBackBattleArea(void)
{
	// ����
	USER_INFO	infoPlayer;		// �ΏۃI�u�W�F�N�g
	for (int cntPlayer = 0; cntPlayer < charcterMax; ++cntPlayer)
	{
		// �ΏۃI�u�W�F�N�g���擾
		infoPlayer = userInfo[cntPlayer];

		// �Ώۂ̃X�e�[�g���m�F
		if (NeedsSkipPlayer(cntPlayer))
		{
			continue;
		}

		// �����߂�
		VECTOR3	vectoruserInfoToCenter = Ground->Pos() - infoPlayer.pos;
		vectoruserInfoToCenter.y = 0.0f;
		float	distanceFromCenter = vectoruserInfoToCenter.x * vectoruserInfoToCenter.x + vectoruserInfoToCenter.y * vectoruserInfoToCenter.y + vectoruserInfoToCenter.z * vectoruserInfoToCenter.z;
		if (distanceFromCenter > (RADIUS_AREA_BATTLE - RADIUS_PUSH_CHARACTER) * (RADIUS_AREA_BATTLE - RADIUS_PUSH_CHARACTER))
		{
			float	distancePushBack = sqrtf(distanceFromCenter) - (RADIUS_AREA_BATTLE - RADIUS_PUSH_CHARACTER);
			vectoruserInfoToCenter.Normalize();
			infoPlayer.pos += vectoruserInfoToCenter * distancePushBack;
		}
		userInfo[cntPlayer].pos = infoPlayer.pos;
	}
}

//==============================================================================
// �v���C���[����X�L�b�v
//==============================================================================
bool NeedsSkipPlayer(int index)
{
	// �G���[�`�F�b�N
	if (index < 0 || index >= charcterMax)
	{
		return true;
	}

	// �X�e�[�g���m�F
#if 0
	int		stateuserInfo = userInfo->State();
	if (stateuserInfo == PLAYER_STATE_DEATH || stateuserInfo == PLAYER_STATE_RESPAWN)
	{
		return true;
	}
#endif
	// �X�L�b�v���Ȃ�
	return false;
}

//==============================================================================
// �C�e����X�L�b�v
//==============================================================================
bool NeedsSkipBullet(int index)
{
#if 0
	// �G���[�`�F�b�N
	if (index < 0 || index >= charcterMax)
	{
		return true;
	}
#endif
	// �X�L�b�v���Ȃ�
	return false;
}

//	EOF