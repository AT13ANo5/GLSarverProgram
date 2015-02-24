//=============================================================================
//	mainファイル [ main.cpp ]
//	Auther : KOTARO NAGASAKI
//=============================================================================

//*****************************************************************************
//	定数定義
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

//*****************************************************************************
//	インクルード
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
//	ライブラリのリンク
//*****************************************************************************
#pragma comment ( lib , "ws2_32.lib" )
#pragma comment (lib, "winmm.lib")

//*****************************************************************************
//	プロトタイプ宣言
//*****************************************************************************
unsigned __stdcall aiUpdate(void *p);

//*****************************************************************************
//	定数定義
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

const float	RADIUS_DEFENSE_CHARACTER = 12.0f;	// キャラクターの防御半径
const float	HEIGHT_DEFENSE_CHARACTER = 0.0f;	// キャラクターの防御中心高さ
const float	RADIUS_OFFENSE_BULLET = 10.0f;		// 砲弾の攻撃半径
const float	HEIGHT_OFFENSE_BULLET = 0.0f;		// 砲弾の攻撃中心高さ
const float	RADIUS_PUSH_CHARACTER = 10.0f;		// キャラクターの押し戻し半径
const float	HEIGHT_PUSH_CHARACTER = 0.0f;		// キャラクターの押し戻し中心高さ
const float	RADIUS_DEFENSE_ROCK = 28.0f;		// 岩の防御半径
const float	HEIGHT_DEFENSE_ROCK = 0.0f;			// 岩の防御中心高さ
const float	RADIUS_PUSH_ROCK = 38.0f;			// 岩の押し戻し半径
const float	HEIGHT_PUSH_ROCK = 0.0f;			// 岩の押し戻し中心高さ
const float FIELD_PANEL_SIZE = 35.0f;			//フィールドのパネル一枚のサイズ
const float	HEIGHT_PLAYER_TO_FIELD = 10.0f;		// プレイヤーと地面の差
const float	RADIUS_AREA_BATTLE = 1000.0f;		// 戦闘エリア半径

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

CMeshGround*	Ground;		// フィールド

//	現在のキャラクター数
int charNum = -1;

//=============================================================================
//	自作バインド関数
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
//	ユーザー情報リセット処理
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
//	タイマー送信処理
//=============================================================================
void sendTimer()
{
	NET_DATA data;

	data.type = DATA_TYPE_TIMER;
	data.servID = SERV_ID;

	//	マルチキャストで送信（送信先で自分のデータだったら勝手にはじけ）
	sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));
}
//=============================================================================
//	リザルトへの移行送信処理
//=============================================================================
void sendChangeResult()
{
	NET_DATA data;

	data.type = DATA_TYPE_CHANGE_RESULT;
	data.charNum = 0;
	data.servID = SERV_ID;

	//	マルチキャストで送信（送信先で自分のデータだったら勝手にはじけ）
	sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));
}
//=============================================================================
//	ai用位置セット＆送信処理
//=============================================================================
void aiSetPos(int _charNum, VECTOR3 _pos)
{
	NET_DATA data;

	//	位置情報のセット
	userInfo[_charNum].pos.x = _pos.x;
	userInfo[_charNum].pos.y = _pos.y;
	userInfo[_charNum].pos.z = _pos.z;

	data.charNum = _charNum;
	data.type = DATA_TYPE_POS;
	data.servID = SERV_ID;
	data.data_pos.posX = _pos.x;
	data.data_pos.posY = _pos.y;
	data.data_pos.posZ = _pos.z;

	//	マルチキャストで送信（送信先で自分のデータだったら勝手にはじけ）
	sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));
}

//=============================================================================
//	ai用位置セット＆送信処理
//=============================================================================
void aiSetRot(int _charNum, VECTOR3 _rot)
{
	NET_DATA data;

	//	位置情報のセット
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

	//	マルチキャストで送信（送信先で自分のデータだったら勝手にはじけ）
	sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));
}

//=============================================================================
//	ai用位置セット＆送信処理
//=============================================================================
void aiSetCannonRot(int _charNum, VECTOR3 _cannonRot)
{
	NET_DATA data;

	//	位置情報のセット
	userInfo[_charNum].cannonRot.x = _cannonRot.x;
	userInfo[_charNum].cannonRot.y = _cannonRot.y;
	userInfo[_charNum].cannonRot.z = _cannonRot.z;

	data.charNum = _charNum;
	data.type = DATA_TYPE_CANNONROT;
	data.servID = SERV_ID;
	data.data_cannonRot.rotX = _cannonRot.x;
	data.data_cannonRot.rotY = _cannonRot.y;
	data.data_cannonRot.rotZ = _cannonRot.z;

	//	マルチキャストで送信（送信先で自分のデータだったら勝手にはじけ）
	sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));
}

//=============================================================================
//	ai用位置セット＆送信処理
//=============================================================================
void aiSetCannon(int _charNum, bool _flag)
{
	NET_DATA data;

	//	位置情報のセット
	userInfo[_charNum].cannon = _flag;

	data.charNum = _charNum;
	data.type = DATA_TYPE_CANNON;
	data.servID = SERV_ID;
	data.data_cannon.flag = _flag;

	//	マルチキャストで送信（送信先で自分のデータだったら勝手にはじけ）
	sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));
}
//=============================================================================
//	リセット処理関数
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
//	メイン処理関数
//=============================================================================
int main(void)
{
	WSADATA wsaData;

	// ホスト名・IPアドレス
	char hostName[hostNameMax];
	char ip[16];
	PHOSTENT phostent;
	IN_ADDR in;

	gameStartFlag = false;

	charNum = -1;

	//	Winsockの初期化
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NULL)
	{
		printf("Winsockの初期化失敗\nエラーコード : %d\n", WSAGetLastError());

		return false;
	}

	//	自分のホスト名取得
	gethostname(hostName, sizeof(hostName));
	phostent = gethostbyname(hostName);

	//	自分のIPアドレスを表示（ユーザーは表示されているIPアドレスを"address.txt"にセットする）
	memcpy(&in, phostent->h_addr, 4);
	sprintf_s(ip, inet_ntoa(in));
	printf("%s:%s\n", hostName, ip);

	//	送信時用変数群生成
	//-------------------------------------------------
	//	ソケットの生成
	sendSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//	送信先アドレス
	sendAdd.sin_port = htons(2000);
	sendAdd.sin_family = AF_INET;
	sendAdd.sin_addr.s_addr = inet_addr("239.0.0.23");//マルチキャストアドレス

	int param = 1;
	int ret = setsockopt(sendSock, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&param, sizeof(param));
	ret = WSAGetLastError();
	//-------------------------------------------------

	//	受信時用変数群生成
	//-------------------------------------------------
	//	ソケットの生成
	SOCKET recvSock;
	recvSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//	受信先のアドレス
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

	// 地形生成
	Ground = CMeshGround::Create(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR2(FIELD_PANEL_SIZE, FIELD_PANEL_SIZE), VECTOR2(0, 0), 1.5f);

	for (;;)
	{
		//	受信
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
				//	データタイプによって分岐
				switch (data.type)
				{
				case DATA_TYPE_POS:

					if (userInfo[data.charNum].entryFlag == true)
					{
						//	位置情報のセット
						userInfo[data.charNum].pos.x = data.data_pos.posX;
						userInfo[data.charNum].pos.y = data.data_pos.posY;
						userInfo[data.charNum].pos.z = data.data_pos.posZ;

						//	マルチキャストで送信（送信先で自分のデータだったら勝手にはじけ）
						sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));
					}

					if (userInfo[data.charNum].deathFlag == true)
					{
						if (userInfo[data.charNum].entryFlag == false)
						{
							//	位置情報のセット
							userInfo[data.charNum].pos.x = data.data_pos.posX;
							userInfo[data.charNum].pos.y = data.data_pos.posY;
							userInfo[data.charNum].pos.z = data.data_pos.posZ;

							//	マルチキャストで送信（送信先で自分のデータだったら勝手にはじけ）
							sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));
						}
					}

					break;

				case DATA_TYPE_ROT:

					if (userInfo[data.charNum].entryFlag == true)
					{
						//	回転情報のセット
						userInfo[data.charNum].rot.y = data.data_rot.rotY;

						//	マルチキャストで送信（送信先で自分のデータだったら勝手にはじけ）
						sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));
					}

					if (userInfo[data.charNum].deathFlag == true)
					{
						if (userInfo[data.charNum].entryFlag == false)
						{
							//	回転情報のセット
							userInfo[data.charNum].rot.y = data.data_rot.rotY;
						}
					}

					break;

				case DATA_TYPE_CANNONROT:

					if (userInfo[data.charNum].entryFlag == true)
					{
						//	回転情報のセット
						userInfo[data.charNum].cannonRot.x = data.data_cannonRot.rotX;
						userInfo[data.charNum].cannonRot.y = data.data_cannonRot.rotY;
						userInfo[data.charNum].cannonRot.z = data.data_cannonRot.rotZ;

						//	マルチキャストで送信（送信先で自分のデータだったら勝手にはじけ）
						sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));
					}

					if (userInfo[data.charNum].deathFlag == true)
					{
						if (userInfo[data.charNum].entryFlag == false)
						{
							//	回転情報のセット
							userInfo[data.charNum].cannonRot.x = data.data_cannonRot.rotX;
							userInfo[data.charNum].cannonRot.y = data.data_cannonRot.rotY;
							userInfo[data.charNum].cannonRot.z = data.data_cannonRot.rotZ;
						}
					}

					break;

				case DATA_TYPE_CANNON:

					if (userInfo[data.charNum].entryFlag == true)
					{
						//	発射フラグのセット
						userInfo[data.charNum].cannon = data.data_cannon.flag;

						//	マルチキャストで送信（送信先で自分のデータだったら勝手にはじけ）
						sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));
					}

					break;

				case DATA_TYPE_KILL:

					//	殺した数インクリメント
					userInfo[data.charNum].kill = data.data_killDeath.value;

					//	マルチキャストで送信（送信先で自分のデータだったら勝手にはじけ）
					sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));

					break;

				case DATA_TYPE_DEATH:

					//	殺された数インクリメント
					userInfo[data.charNum].death = data.data_killDeath.value;

					//	マルチキャストで送信（送信先で自分のデータだったら勝手にはじけ）
					sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));

					break;


				case DATA_TYPE_SEND_DEATH:

					userInfo[data.charNum].deathFlag = true;

					break;

				case DATA_TYPE_SEND_REBORN:

					userInfo[data.charNum].deathFlag = false;

					break;

				case DATA_TYPE_PAUSE:

					//	ポーズ時にデータを渡す
					//	指定のキャラクタのところにのみ送信
					for (int count = 0; count < charcterMax; count++)
					{
						//	自分以外の場合
						if (count != data.charNum)
						{
							data.charNum = count;

							//	送信データ格納
							data.servID = SERV_ID;
							data.type = DATA_TYPE_PAUSE;
							data.data_pause.kill = userInfo[count].kill;
							data.data_pause.death = userInfo[count].death;

							//	マルチキャストで送信（送信先で自分のデータだったら勝手にはじけ）
							sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));
						}
					}

					break;

				case DATA_TYPE_GAME_START:

					//	最上位クライアントから、ゲーム開始を受け取ったら
					if (data.charNum == 0)
					{
						AI::Initialize(charNum + 1);

						// AI処理用スレッド開始
						if (ai == 0)
						{
							ai = (HANDLE)_beginthreadex(NULL,0,&aiUpdate,NULL,NULL,NULL);
						}

						for (int count = charNum + 1; count < charcterMax; count++)
						{
							//	ユーザー情報をセット
							userInfo[count].fromaddr = recvAdd;
							userInfo[count].fromaddr.sin_port = htons(3000);
							userInfo[count].entryFlag = false;
							userInfo[count].deathFlag = true;
						}

						//	ゲームスタートさせる
						gameStartFlag = true;
						printf("GameStart!\n");
					}

					break;

				case DATA_TYPE_CHANGE_GAME:

					//	最上位クライアントから、ゲームへの遷移を受け取ったら
					if (data.charNum == 0)
					{
						//	他のクライアントにもゲームへの遷移を伝える
						//	マルチキャストで送信（送信先で自分のデータだったら勝手にはじけ）
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

					//	ポートを再設定
					recvAdd.sin_port = htons(3000);

					//	マルチキャストで送信（送信先で自分のデータだったら勝手にはじけ）
					sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));

					break;

				case DATA_TYPE_ENTRY:

					//	ゲームスタートしていなければ
					if (gameStartFlag == false)
					{
						//	エントリー処理
						//	現在のプレイヤー数を返し、自分のプレイヤー番号をセットさせる。
						{
							//	ループ処理判定フラグ
							bool errorFlag = true;

							//	キャラクター番号をインクリメント（キャラクター番号は0～5）
							charNum++;

							//	キャラクター数が一定値を超えていたら処理をはじく
							if (charNum < charcterMax)
							{
								//	エラー処理（エントリー状態ならエラーとする）
								if (userInfo[charNum].entryFlag == true)
								{
									//	キャラクタ番号をマイナスし、処理を弾く
									charNum--;
									errorFlag = false;

									break;
								}

								//	エラー処理を抜けたら
								if (errorFlag == true)
								{
									//	数値をセット
									data.charNum = charNum;
									data.servID = SERV_ID;
									data.type = DATA_TYPE_ENTRY;

									//	ユーザー情報をセット
									userInfo[charNum].fromaddr = recvAdd;
									userInfo[charNum].fromaddr.sin_port = htons(3000);
									userInfo[charNum].entryFlag = true;

									//	ポートを再設定
									recvAdd.sin_port = htons(3000);

									printf("Player%d Entry\n",charNum+1);

									//	「エントリーした」という情報をマルチキャストで送信
									sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));
								}
							}
							else
							{
								data.type = DATA_TYPE_EMPTY;
								data.servID = SERV_ID;

								//	ポートを再設定
								recvAdd.sin_port = htons(3000);

								//	部屋がいっぱいと送る
								//	マルチキャストで送信（送信先で自分のデータだったら勝手にはじけ）
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

	// 地形の破棄
	SafeDelete(Ground);

	CloseHandle(ai);
	ai = 0;
	// ソケット終了
	closesocket(recvSock);

	// WinSock終了処理
	WSACleanup();

	AI::Finalize();
}

//=============================================================================
//	AI用更新処理関数
//=============================================================================
unsigned __stdcall aiUpdate(void *p)
{
	unsigned int CurrentTime = 0;//DWORD dwCurrentTime;
	unsigned int PrevTime = 0;	 //DWORD dwExecLastTime;
	unsigned int FPSLastTime = 0;//DWORD dwFPSLastTime;
	unsigned int FrameCount = 0; //DWORD dwFrameCount;

	threadEndFlag = false;

	timeBeginPeriod(1);				// 分解能を設定

	FPSLastTime =
		CurrentTime = timeGetTime();

	PrevTime = 0;

	FrameCount = 0;

	if (charNum == 0)
	{
		userInfo[1].entryFlag = false;
	}

	//	クライアント側とFPSをそろえる
	for (;;)
	{
		CurrentTime = timeGetTime();
		if ((CurrentTime - FPSLastTime) >= 500)	// 0.5秒ごとに実行
		{
			FPSLastTime = CurrentTime;
			FrameCount = 0;
		}

		if (CurrentTime - PrevTime >= 1000 / 60)
		{
			//	AI更新処理
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
// キャラクター同士の押し戻し
//==============================================================================
void PushBackCharacter(void)
{
	// 攻撃側の決定
	USER_INFO	infoOffense;		// 攻撃側プレイヤー
	for (int cntOffense = 0; cntOffense < charcterMax; ++cntOffense)
	{
		// プレイヤーを取得
		infoOffense = userInfo[cntOffense];

		// プレイヤーが判定可能か確認
		if (NeedsSkipPlayer(cntOffense))
		{
			continue;
		}

		// 防御側の決定
		for (int cntDefense = 0; cntDefense < charcterMax; ++cntDefense)
		{
			// プレイヤーを取得
			USER_INFO	infoDefense = userInfo[cntDefense];		// 防御側プレイヤー

			// プレイヤーが判定可能か確認
			if (NeedsSkipPlayer(cntDefense))
			{
				continue;
			}
			if (cntOffense == cntDefense)
			{
				continue;
			}

			// 当たり判定
			VECTOR3	positionOffense = infoOffense.pos;		// 攻撃判定中心座標
			VECTOR3	positionDefense = infoDefense.pos;		// 防御判定中心座標
			VECTOR3	vectorOffenseToDefense;					// 攻撃判定から防御判定へのベクトル
			float	distanceOffenseAndDefense;				// 判定の中心同士の距離
			positionOffense.y += HEIGHT_PUSH_CHARACTER;
			positionDefense.y += HEIGHT_PUSH_CHARACTER;
			vectorOffenseToDefense = positionDefense - positionOffense;
			distanceOffenseAndDefense = sqrtf(vectorOffenseToDefense.x * vectorOffenseToDefense.x + vectorOffenseToDefense.y * vectorOffenseToDefense.y + vectorOffenseToDefense.z * vectorOffenseToDefense.z);
			if (distanceOffenseAndDefense < 2.0f * RADIUS_PUSH_CHARACTER)
			{
				// 押し戻し
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

				// エフェクト：火花　プレイヤー同士のぶつかり
			}
			userInfo[cntDefense].pos = infoDefense.pos;
		}
		userInfo[cntOffense].pos = infoOffense.pos;
	}
}

//==============================================================================
// キャラクターと岩の押し戻し
//==============================================================================
void PushBackRock(void)
{
	// 攻撃側の決定
	USER_INFO	infoPlayer;		// 攻撃側プレイヤー
	for (int cntPlayer = 0; cntPlayer < charcterMax; ++cntPlayer)
	{
		// プレイヤーを取得
		infoPlayer = userInfo[cntPlayer];

		// プレイヤーが判定可能か確認
		if (NeedsSkipPlayer(cntPlayer))
		{
			continue;
		}

		// 防御側の決定
		VECTOR3	positioninfoPlayer = infoPlayer.pos;						// 攻撃判定中心座標
		int		maxRockData = sizeof(ROCK_DATA_LIST) / sizeof(ROCK_DATA);	// 最大岩数
		for (int cntRock = 0; cntRock < maxRockData; ++cntRock)
		{
			// 当たり判定
			VECTOR3	positionRock = ROCK_DATA_LIST[cntRock].position;	// 防御判定中心座標
			VECTOR3	vectorOffenseToDefense;								// 攻撃判定から防御判定へのベクトル
			float	distanceOffenseAndDefense;							// 判定の中心同士の距離
			float	scalingRock;										// 岩の大きさ
			scalingRock = (ROCK_DATA_LIST[cntRock].scaling.x < ROCK_DATA_LIST[cntRock].scaling.z ? ROCK_DATA_LIST[cntRock].scaling.x : ROCK_DATA_LIST[cntRock].scaling.z);
			positioninfoPlayer.y += HEIGHT_PUSH_CHARACTER;
			positionRock.y += HEIGHT_PUSH_ROCK;
			vectorOffenseToDefense = positionRock - positioninfoPlayer;
			distanceOffenseAndDefense = sqrtf(vectorOffenseToDefense.x * vectorOffenseToDefense.x + vectorOffenseToDefense.y * vectorOffenseToDefense.y + vectorOffenseToDefense.z * vectorOffenseToDefense.z);

			if (distanceOffenseAndDefense < RADIUS_PUSH_CHARACTER + RADIUS_PUSH_ROCK * scalingRock)
			{
				// 押し戻し
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
// 地形の押し戻し
//==============================================================================
void PushBackField(void)
{
	// 判定
	USER_INFO	infoPlayer;		// 対象オブジェクト
	for (int cntPlayer = 0; cntPlayer < charcterMax; ++cntPlayer)
	{
		// 対象オブジェクトを取得
		infoPlayer = userInfo[cntPlayer];

		// 対象のステートを確認
		if (NeedsSkipPlayer(cntPlayer))
		{
			continue;
		}

		// 押し戻し
		PushBackObjectByField(&userInfo[cntPlayer].pos);
	}
}

//==============================================================================
// オブジェクトの地形による押し戻し
//==============================================================================
void PushBackObjectByField(VECTOR3* pPosition)
{
	// 地形とのあたり判定
	VECTOR3	NormalGround;		// 地形の法線
	pPosition->y = Ground->GetHeight(*pPosition, &NormalGround) + HEIGHT_PLAYER_TO_FIELD;;
}

//==============================================================================
// 戦闘範囲押し戻し
//==============================================================================
void PushBackBattleArea(void)
{
	// 判定
	USER_INFO	infoPlayer;		// 対象オブジェクト
	for (int cntPlayer = 0; cntPlayer < charcterMax; ++cntPlayer)
	{
		// 対象オブジェクトを取得
		infoPlayer = userInfo[cntPlayer];

		// 対象のステートを確認
		if (NeedsSkipPlayer(cntPlayer))
		{
			continue;
		}

		// 押し戻し
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
// プレイヤー判定スキップ
//==============================================================================
bool NeedsSkipPlayer(int index)
{
	// エラーチェック
	if (index < 0 || index >= charcterMax)
	{
		return true;
	}

	// ステートを確認
#if 0
	int		stateuserInfo = userInfo->State();
	if (stateuserInfo == PLAYER_STATE_DEATH || stateuserInfo == PLAYER_STATE_RESPAWN)
	{
		return true;
	}
#endif
	// スキップしない
	return false;
}

//==============================================================================
// 砲弾判定スキップ
//==============================================================================
bool NeedsSkipBullet(int index)
{
#if 0
	// エラーチェック
	if (index < 0 || index >= charcterMax)
	{
		return true;
	}
#endif
	// スキップしない
	return false;
}

//	EOF