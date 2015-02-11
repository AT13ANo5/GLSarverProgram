//=============================================================================
//	mainファイル [ main.h ]
//	Auther : KOTARO NAGASAKI
//=============================================================================
#ifndef _NET_DATA_H_
#define _NET_DATA_H_

//*****************************************************************************
//	インクルード
//*****************************************************************************
#include <winsock2.h>
#include "CommonGL.h"
//*****************************************************************************
//	列挙体定義
//*****************************************************************************

//	データ内容判別正数値
//----------------------------------------------------------------
typedef enum
{

	DATA_TYPE_POS = 100,	//	位置
	DATA_TYPE_ROT,	//	回転
	DATA_TYPE_CANNONROT,	//	回転
	DATA_TYPE_CANNON,	//	発射
	DATA_TYPE_ENTRY,	//	エントリー
	DATA_TYPE_GET_ENTRY,
	DATA_TYPE_CHANGE_GAME,
	DATA_TYPE_CHANGE_RESULT,
	DATA_TYPE_GAME_START,	//	ゲームスタート
	DATA_TYPE_DEATH,	//	死んだ
	DATA_TYPE_KILL,	//	殺した
	DATA_TYPE_PAUSE,	//	ポーズ状態
	DATA_TYPE_EMPTY,	//	満杯
	DATA_TYPE_MAX	//	最大値
}DATA_TYPE;
//----------------------------------------------------------------

//*****************************************************************************
//	構造体定義
//*****************************************************************************

//	位置情報
//----------------------------------------------------------------
typedef struct
{
	float posX;
	float posY;
	float posZ;

}DATA_POS;
//----------------------------------------------------------------

//	回転情報
//----------------------------------------------------------------
typedef struct
{
	float rotX;
	float rotY;
	float rotZ;

}DATA_ROT;
//----------------------------------------------------------------

//	発射フラグ
//----------------------------------------------------------------
typedef struct
{
	bool flag;

}DATA_CANNON;
//----------------------------------------------------------------

//	ポーズ時用データ
//----------------------------------------------------------------
typedef struct
{
	int kill;	//	指定のプレイヤーの殺した数をセット
	int death;	//	指定のプレイヤーの殺された数をセット

}DATA_PAUSE;
//----------------------------------------------------------------

//	コネクション用データ
//----------------------------------------------------------------
typedef struct
{
	bool entryFlag[6];	//	エントリーしてるかどうか
}DATA_CONNECTION;
//----------------------------------------------------------------

//	kill death用データ
//----------------------------------------------------------------
typedef struct
{
	int value;
}DATA_KILLDEATH;
//----------------------------------------------------------------

//	送信時用データ
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

// ユーザー情報
//----------------------------------------------------------------
typedef struct {
	sockaddr_in fromaddr;	// アドレス
	bool entryFlag;	//	エントリーフラグ
	int kill;	//	殺した数
	int death;	//	殺された数

	VECTOR3 pos;
	VECTOR3 rot;
	VECTOR3 cannonRot;
	bool cannon;

} USER_INFO;
//----------------------------------------------------------------

// 岩情報
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
// 当たり判定
//----------------------------------------------------------------
void PushBackCharacter(void);					// キャラクター同士の押し戻し
void PushBackRock(void);						// キャラクターと岩の押し戻し
void PushBackField(void);						// 地形の押し戻し
void PushBackObjectByField(VECTOR3* pPosition);	// オブジェクトの地形による押し戻し
void PushBackBattleArea(void);					// 行動可能範囲の押し戻し
bool NeedsSkipPlayer(int index);				// プレイヤー判定スキップ
bool NeedsSkipBullet(int index);				// 砲弾判定スキップ

#endif

//	EOF