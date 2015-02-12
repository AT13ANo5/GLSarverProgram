//=============================================================================
//	AIクラス
//=============================================================================
#ifndef _AI_H_
#define _AI_H_

//------------------------------------------------------------------------------
// マクロ定義
//------------------------------------------------------------------------------

#define BARREL_ROT_MAX (-60.0f)				// 砲身の最大角度
#define BARREL_ROT_MIN (0.0f)				// 砲身の最小角度

//------------------------------------------------------------------------------
// ヘッダインクルード
//------------------------------------------------------------------------------
#include "main.h"

class CMeshGround;

//------------------------------------------------------------------------------
// クラス宣言
//------------------------------------------------------------------------------
class AI
{
public:
	AI();
	~AI();
	static void Initialize(int _charNum);
	static void Finalize(void);

	static void UpdateAll(void);
	static void UpdateInfomation(void);
	static void SendState(void);
	// 現在の装填時間ゲッター
	int ReloadTimer(void){ return _ReloadTimer; }

	// 体力ゲッター
	int PlayerLife(void){ return _PlayerLife; }

	static void SetUserInfo(USER_INFO* info, int _charNum);
private:

	static AI* Top;
	static AI* Cur;
	AI* Next;
	AI* Prev;

	static CMeshGround* Field;

	void LinkList(void);
	void UnlinkList(void);

	void Init(void);
	void Update(void);

	void SurchTarget(void);
	void Shot(void);
	void MazzleRevision(void);

	USER_INFO UserInfo;
	int TargetId;
	VECTOR3 TargetPos;
	VECTOR3 TargetSpeed;
	float DestRotY;
	int ID;

	VECTOR3 Movement;	// 移動量
	float Speed;		// 移動速度
	float BarrelRotX;	// 砲身のX軸回転量
	float Distance;
	bool LaunchFlag;	// 弾発射フラグ（true / 使用：false / 不使用）
	int _ReloadTimer;	// 現在の装填時間
	int _PlayerLife;	// 体力
	int frame;

};

#endif