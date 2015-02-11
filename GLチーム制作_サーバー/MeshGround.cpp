#define _CRT_SECURE_NO_WARNINGS
#include "MeshGround.h"

#define SUM_INDEX(X,Z) ((X+1)*(Z-1)+((X+1)*(Z+1))+(Z-1)*2)
#define HEIGHT_MAP ("heightmap.bmp")

CMeshGround::CMeshGround(int priority) :CObject(priority)
{
	_Pos = VECTOR3(0,0,0);
	_Rot = VECTOR3(0,0,0);
	Vtx = nullptr;
	Tex = nullptr;
	Nor = nullptr;
	NormalMap = nullptr;
	HeightMap = nullptr;
}
CMeshGround::~CMeshGround()
{
	if (Vtx != nullptr)
	{
		delete[] Vtx;
		Vtx = nullptr;
	}
	if (Tex != nullptr)
	{
		delete[] Tex;
		Tex = nullptr;
	}
	if (Nor != nullptr)
	{
		delete[] Nor;
		Nor = nullptr;
	}
	if (NormalMap != nullptr)
	{
		delete[] NormalMap;
		NormalMap = nullptr;
	}
	if (Index != nullptr)
	{
		delete[] Index;
		Index = nullptr;
	}
	if (HeightMap != nullptr)
	{
		delete[] HeightMap;
		HeightMap = nullptr;
	}
}
CMeshGround* CMeshGround::Create(VECTOR3 pos,VECTOR2 PanelSize,VECTOR2 PanelNum,float heightMag)
{
	CMeshGround* Ground = new CMeshGround;
	if (Ground == nullptr)
	{
		return nullptr;
	}
	Ground->_Pos = pos;
	Ground->PanelSize = PanelSize;
	Ground->PanelNum = PanelNum;
	Ground->HeightMag = heightMag;
	Ground->Init();

	return Ground;

}

void CMeshGround::Init(void)
{
	IndexNum = (int)(SUM_INDEX(PanelNum.x,PanelNum.y));
	VertexNum = (int)((PanelNum.x + 1)*(PanelNum.y + 1));
	PolygonNum = (int)(((PanelNum.x * 2)*PanelNum.y) + ((PanelNum.y - 1) * 4));
	MapNum = (int)(PanelNum.x*PanelNum.y * 2);
	LoadImg(HEIGHT_MAP);
	NormalMap = new VECTOR3[MapNum];

	
	Vtx = new VECTOR3[VertexNum];
	Tex = new VECTOR2[VertexNum];
	Nor = new VECTOR3[VertexNum];

	_Size.x = PanelNum.x*PanelSize.x;
	_Size.y = 0;
	_Size.z = PanelNum.y*PanelSize.y;

	float OffsetX = (PanelNum.x*PanelSize.x) / 2;
	float OffsetZ = (PanelNum.y*PanelSize.y) / 2;

	for (int LoopZ = 0,num = 0;LoopZ < PanelNum.y + 1;LoopZ++)
	{
		for (int LoopX = 0;LoopX < PanelNum.x + 1;LoopX++)
		{
			if (num < VertexNum)
			{
				Vtx[num] = VECTOR3(OffsetX + (-PanelSize.x*LoopX),HeightMap[num],-OffsetZ + (PanelSize.y*LoopZ));
				Tex[num] = VECTOR2((float)LoopX,(float)LoopZ);
				Nor[num] = VECTOR3(0,1.0f,0);
				_Color = COLOR(1.0f,1.0f,1.0f,1.0f);
			}
			num++;
		}
	}

	//法線マップを作成
	VECTOR3 Vec1,Vec2,Cross;
	int cnt = 0;
	for (int Z = 0,num = 0;Z < PanelNum.y;Z++)
	{
		for (int X = 0;X < PanelNum.x;X++)
		{
			//四角形が／で分割された上側の法線の計算
			if (num + PanelNum.x + 1 < VertexNum)
			{
				Vec1 = Vtx[(int)(PanelNum.x + 1) + num] - Vtx[num];
				Vec2 = Vtx[1 + num] - Vtx[num];
			}
			VECTOR3::Cross(&Cross,Vec2,Vec1);
			Cross.Normalize();
			if (cnt < MapNum)
			{
				NormalMap[cnt] = Cross;
			}
			cnt++;

			if (num + PanelNum.x + 1 < VertexNum)
			{
				//四角形が／で分割された下側の法線の計算
				Vec1 = Vtx[num + 1] - Vtx[(int)(PanelNum.x + 1) + (num + 1)];
				Vec2 = Vtx[(int)(PanelNum.x + 1) + num] - Vtx[(int)(PanelNum.x + 1) + (num + 1)];
				VECTOR3::Cross(&Cross,Vec2,Vec1);
				Cross.Normalize();
				NormalMap[cnt] = Cross;
			}
			cnt++;
			num++;
		}
		//頂点がX軸の端まで来たので折り返す
		num++;
	}
	cnt = 0;

	//法線を計算しつつ頂点に設定
	for (int Y = 0,num = 0;Y <= PanelNum.y;Y++)
	{//numは面の番号
		for (int X = 0;X <= PanelNum.x;X++)
		{
			VECTOR3 AddCross = VECTOR3(0,0,0);//一時変数を初期化 計算結果格納用
			if (X == 0)
			{//X軸の左端の時
				if (Y == 0)
				{//一番最初の段なら
					AddCross = NormalMap[num] + NormalMap[num + 1];
				}
				else if (Y == PanelNum.y)
				{//一番最後の段
					num = num - (int)PanelNum.x * 2;//一列前の番号に戻す
					AddCross = NormalMap[num];
					num--;//次も同じ番号なので一つ戻しておく
				}
				else if ((num - ((int)PanelNum.x * 2)) >= 0 && num + 1 < MapNum)
				{//中間
					AddCross = NormalMap[num - ((int)PanelNum.x * 2)] + NormalMap[num + 1] + NormalMap[num];
				}
			}
			else
			{
				if (Y == 0)
				{//一番最初の段
					if (X == PanelNum.x && num + 1 < MapNum)
					{//右端の時
						AddCross = NormalMap[num] + NormalMap[num + 1];
					}
					else if (num + 2 < MapNum)
					{
						AddCross = NormalMap[num] + NormalMap[num + 1] + NormalMap[num + 2];
					}
				}
				else if (Y == PanelNum.y)
				{//一番最後の段
					if (X == PanelNum.x)
					{//右端の時
						AddCross = NormalMap[num] + NormalMap[num + 1];
					}
					else
					{
						AddCross = NormalMap[num] + NormalMap[num + 1] + NormalMap[num + 2];
					}
				}
				else if (X == PanelNum.x)
				{//Yが端ではなく、同時に右端だったとき
					AddCross = NormalMap[num] + NormalMap[num - ((int)PanelNum.x * 2)] + NormalMap[num - ((int)PanelNum.x * 2) - 1];
				}
				else
				{
					AddCross = NormalMap[num] + NormalMap[num + 1] + NormalMap[num + 2] + NormalMap[num - ((int)PanelNum.x * 2) - 1] + NormalMap[num - ((int)PanelNum.x * 2)] + NormalMap[num - ((int)PanelNum.x * 2) + 1];
				}
				if (X < PanelNum.x)
				{//右端以外なら面の番号を一つ進める
					num++;
				}
			}

			//正規化した法線データを頂点に設定
			AddCross.Normalize();
			if (cnt < VertexNum)
			{
				Nor[cnt] = AddCross;
			}
			cnt++;//頂点番号を進める
			num++;//面の番号を進める
		}

	}
	
	int LoopX = 0;
	int VtxNo = 0;
	Index = new int[IndexNum];
	for (int cnt = 0;cnt < IndexNum;cnt++)
	{
		Index[cnt] = 0;
	}
	for (int LoopZ = 0;LoopZ < PanelNum.y;LoopZ++)
	{
		if (LoopZ != 0)
		{
			LoopX = 0;
			if (VtxNo < IndexNum)
			{
				Index[VtxNo] = (int)((LoopZ*(PanelNum.x + 1)) + (((LoopX + 1) % 2)*(PanelNum.x + 1) + (LoopX / 2)));
			}
			VtxNo++;
		}
		for (LoopX = 0;LoopX < (PanelNum.x + 1) * 2;LoopX++)
		{
			if (VtxNo < IndexNum)
			{
				Index[VtxNo] = (int)((LoopZ*(PanelNum.x + 1)) + (((LoopX + 1) % 2)*(PanelNum.x + 1) + (LoopX / 2)));
			}
			VtxNo++;
		}
		if (LoopZ == PanelNum.y - 1)
		{
			break;
		}
		if (VtxNo > 0 && VtxNo < IndexNum)
		{
			Index[VtxNo] = Index[VtxNo - 1];
		}
		VtxNo++;
	}


}

void CMeshGround::Uninit(void)
{
	delete this;
}
void CMeshGround::Update(void)
{
	//Rot.x++;

}

void CMeshGround::GetPanelIndex(VECTOR3 pos,int* OutIndexX,int* OutIndexY)
{
	pos.x = _Size.x / 2 + (pos.x);
	pos.z = _Size.z / 2 + (pos.z);

	*OutIndexX = (int)(PanelNum.x - pos.x / PanelSize.x);
	*OutIndexY = (int)(pos.z / PanelSize.y);

}

float CMeshGround::GetHeight(VECTOR3 pos,VECTOR3* normal)
{
	int IndexX = 0;
	int IndexY = 0;
	GetPanelIndex(pos,&IndexX,&IndexY);
	int Index = (int)(IndexX + (IndexY*(PanelNum.x + 1)));
	VECTOR3 VertexPos[4];
	VertexPos[0] = VECTOR3((0.5f * PanelNum.x - IndexX) * PanelSize.x,HeightMap[Index],(-0.5f * PanelNum.y + IndexY) * PanelSize.y);
	VertexPos[1] = VECTOR3((0.5f * PanelNum.x - IndexX - 1) * PanelSize.x,HeightMap[Index + 1],(-0.5f * PanelNum.y + IndexY) * PanelSize.y);
	VertexPos[2] = VECTOR3((0.5f * PanelNum.x - IndexX - 1) * PanelSize.x,HeightMap[Index + ((int)PanelNum.x + 2)],(-0.5f * PanelNum.y + IndexY + 1) * PanelSize.y);
	VertexPos[3] = VECTOR3((0.5f * PanelNum.x - IndexX) * PanelSize.x,HeightMap[Index + ((int)PanelNum.x + 1)],(-0.5f * PanelNum.y + IndexY + 1) * PanelSize.y);

#if 0
	for (int cnt = 0;cnt<4;cnt++)
	{
		VertexPos[cnt].x -= _Size.x/2;
		VertexPos[cnt].z -= _Size.z/2;
		VertexPos[cnt].z *= -1;
	}
#endif
	VECTOR3 Vec0 = VertexPos[1] - VertexPos[0];
	VECTOR3 Vec1 = VECTOR3(0,0,0);

	bool flag = false;
	for (int cnt = 0;cnt < 3;cnt++)
	{
		Vec0 = VertexPos[((cnt + 1) % 3)] - VertexPos[(cnt % 3)];
		Vec1 = pos - VertexPos[(cnt % 3)];
		if ((Vec0.z*Vec1.x - Vec0.x*Vec1.z) >= 0)
		{
			flag = true;
		}
		else
		{
			flag = false;
			break;
		}
	}
	if (flag)
	{

		return GetHeightPolygon(VertexPos[1],VertexPos[2],VertexPos[0],pos,normal);
	}
	else
	{

		return GetHeightPolygon(VertexPos[3],VertexPos[0],VertexPos[2],pos,normal);
	}

	return 0;
}

float CMeshGround::GetHeightPolygon(const VECTOR3& p0,const VECTOR3& p1,const VECTOR3& p2,VECTOR3& pos,VECTOR3* Normal)
{

	VECTOR3 Vec1,Vec0;
	VECTOR3 normal = VECTOR3(0,0,0);
	Vec0 = p1 - p0;
	Vec1 = p2 - p0;

	VECTOR3::Cross(&normal,Vec0,Vec1);
	normal.Normalize();
	if (normal.y == 0.0f)
	{
		return 0;
	}
	*Normal = normal;

	float Height = p0.y - (-normal.x*(pos.x - p0.x) + normal.z*(pos.z - p0.z)) / normal.y;

	return Height;

}

//画像データ読み込み
void CMeshGround::LoadImg(const char * imgFile)
{
	FILE *file;
	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER bmih;//ヘッダー情報
	float FieldScl = HeightMag;
	file = fopen(imgFile,"rb");
	if (file != NULL)
	{
		fread(&bmfh,sizeof(BITMAPFILEHEADER),1,file);
		fread(&bmih,sizeof(BITMAPINFOHEADER),1,file);
		if (PanelNum.x <= 0 || PanelNum.y <= 0)
		{
			PanelNum.x = bmih.biWidth - 1;
			PanelNum.y = bmih.biHeight - 1;
		}
   if(PanelNum.x > bmih.biWidth)
   {
    PanelNum.x = bmih.biWidth;
   }
   if(PanelNum.y > bmih.biHeight)
   {
    PanelNum.y = bmih.biHeight;
   }
		IndexNum = (int)(SUM_INDEX(PanelNum.x,PanelNum.y));
		VertexNum = (int)((PanelNum.x + 1)*(PanelNum.y + 1));
		PolygonNum = (int)(((PanelNum.x * 2)*PanelNum.y) + ((PanelNum.y - 1) * 4));
		MapNum = (int)(PanelNum.x*PanelNum.y * 2);

		HeightMap = new float[VertexNum];
		bmih.biWidth = PanelNum.x + 1;
		bmih.biHeight = PanelNum.y + 1;
		BYTE Height;
		if (bmih.biBitCount == 24)
		{

			for (int y = bmih.biHeight - 1; y >= 0; y--)
			{
				for (int x = 0; x < bmih.biWidth; x++)
				{
					fread(&Height,1,1,file);
					fread(&Height,1,1,file);
					fread(&Height,1,1,file);
					if (Height == '\0')
					{

						fread(&Height,1,1,file);
						fread(&Height,1,1,file);
						fread(&Height,1,1,file);
					}
					HeightMap[bmih.biWidth * y + x] = (float)Height - 255;
					HeightMap[bmih.biWidth * y + x] += 255;
					HeightMap[bmih.biWidth * y + x] *= FieldScl;
				}
			}
			ImgSize.x = bmih.biWidth;
			ImgSize.y = bmih.biHeight;
		}
		else
			if (bmih.biBitCount == 8)
			{
				RGBQUAD sRGB[256];

				fread(&sRGB,sizeof(RGBQUAD),256,file);

				for (int y = bmih.biHeight - 1; y >= 0; y--)
				{
					for (int x = 0; x < bmih.biWidth; x++)
					{
						fread(&Height,1,1,file);
						if (Height == '\0')
						{

							fread(&Height,1,1,file);
						}

						HeightMap[bmih.biWidth * y + x] = (float)sRGB[Height].rgbBlue - 255;
						HeightMap[bmih.biWidth * y + x] += 255;
						HeightMap[bmih.biWidth * y + x] *= FieldScl;
					}
				}
				ImgSize.x = bmih.biWidth;
				ImgSize.y = bmih.biHeight;


			}
		fclose(file);

	}
	else{

		//失敗したら10x10の平面
		HeightMap = new float[11 * 11];
		memset(HeightMap,0,sizeof(float));
		ImgSize.x = 10;
		ImgSize.y = 10;
		PanelNum.x = ImgSize.x - 1;
		PanelNum.y = ImgSize.y - 1;
	}

}