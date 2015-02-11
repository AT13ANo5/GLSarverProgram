//=============================================================================
//起伏付きフィールドクラス
//=============================================================================
#ifndef _SCENE_GROUND_H_
#define _SCENE_GROUND_H_
//=============================================================================
//インクルード
//=============================================================================
#include "CommonGL.h"

typedef struct
{
	VECTOR3 nor;
	VECTOR3 pos;

}VERTEX;

//クラス定義
class CMeshGround
{
public:
	CMeshGround(void);
	~CMeshGround();
	//=============================================================================
	//作成
	//引数：
	//	VECTOR3：中心座標
	//	VECTOR2：ポリゴン一枚のサイズ
	//	VECTOR2:分割数
	//=============================================================================
	static CMeshGround* Create(VECTOR3 pos,VECTOR2 PanelSize,VECTOR2 PanelNum,float heightMag = 2.0f);

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//=============================================================================
	//高さを取得
	//=============================================================================
	float GetHeight(VECTOR3 pos,VECTOR3* normal = nullptr);

	//ゲッター
	VECTOR3 Size(void)const{ return _Size; }
	VECTOR3 Pos(void)const{ return _Pos; }
	
private:

	VECTOR3 _Pos;

	VECTOR3 _Size;
	
	VECTOR2 PanelSize;
	VECTOR2 PanelNum;
	VECTOR2 ImgSize;
	int IndexNum;
	int PolygonNum;
	int VertexNum;
	float HeightMag;//高さの倍率

	VECTOR3* Vtx;
	VECTOR3* Nor;

	VECTOR3* NormalMap;
	float* HeightMap;
	int* Index;
	int MapNum;

	void GetPanelIndex(VECTOR3 pos,int* OutIndexX,int* OutIndexY);
	float GetHeightPolygon(const VECTOR3& p0,const VECTOR3& p1,const VECTOR3& p2,VECTOR3& pos,VECTOR3* Normal = nullptr);
	void LoadImg(const char * imgFile);//画像データ読み込み

};


#endif