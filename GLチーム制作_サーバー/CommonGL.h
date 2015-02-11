//===================================================================
//汎用構造体群
//===================================================================
#ifndef _COMMONGL_H_
#define _COMMONGL_H_

#include <stdio.h>
#include <process.h>

//*****************************************************************************
//	ライブラリのリンク
//*****************************************************************************
//デリートを簡略化したマクロ
#define SafeDelete(x) if(x!=nullptr){	\
						delete x;		\
						x = nullptr; }

//デリートを簡略化したマクロ(配列用)
#define SafeDeletes(x) if(x!=nullptr){	\
						delete[] x;		\
						x = nullptr; }
// 角関連
#define PI     (3.141592654f) // 円周率
#define RAD_TO_DEG   (57.29577951f) // 1ラジアン→度
#define DEG_TO_RAD   (0.017453293f) // 1度→ラジアン

// 度→ラジアン
#define DEG2RAD(degree)  ((degree) * DEG_TO_RAD)
// ラジアン→度
#define RAD2DEG(radian)  ((radian) * RAD_TO_DEG)

//===================================================================
//XY
//===================================================================
struct VECTOR2
{
public:
	VECTOR2(){};
	VECTOR2(float fx,float fy);
	VECTOR2 operator + (const VECTOR2&) const;
	VECTOR2 operator - (const VECTOR2&) const;
	VECTOR2 operator * (const VECTOR2&) const;
	VECTOR2 operator * (const float) const;
	VECTOR2 operator / (const VECTOR2&) const;
	VECTOR2 operator / (const float) const;
	VECTOR2 operator += (const VECTOR2&);
	VECTOR2 operator -= (const VECTOR2&);
	VECTOR2 operator *= (const VECTOR2&);
	VECTOR2 operator *= (const float);
	VECTOR2 operator /= (const VECTOR2&);
	VECTOR2 operator /= (const float);

	float Dot(const VECTOR2& Src1,const VECTOR2& Src2);
	float Cross(const VECTOR2& Src1,const VECTOR2& Scr2);

	union
	{
		struct
		{
			float x,y;
		};
		float f[2];
	};
};

//===================================================================
//XYZ
//===================================================================
struct VECTOR3
{
public:
	VECTOR3(){};
	VECTOR3(float fx,float fy,float fz);
	VECTOR3 operator + (const VECTOR3&) const;
	VECTOR3 operator - (const VECTOR3&) const;
	VECTOR3 operator * (const VECTOR3&) const;
	VECTOR3 operator * (const float) const;
	VECTOR3 operator / (const VECTOR3&) const;
	VECTOR3 operator / (const float) const;
	VECTOR3 operator += (const VECTOR3&);
	VECTOR3 operator -= (const VECTOR3&);
	VECTOR3 operator *= (const VECTOR3&);
	VECTOR3 operator *= (float f);
	VECTOR3 operator /= (const VECTOR3&);
	VECTOR3 operator /= (float f);
	void Normalize(void);//正規化
	static float Dot(const VECTOR3& Src1,const VECTOR3& Src2);//内積
	static void Cross(VECTOR3* Output,const VECTOR3& Src1,const VECTOR3& Src2);//外積

	union
	{
		struct
		{
			float x,y,z;
		};
		float f[3];
	};
	
};
//===================================================================
//XYZW
//===================================================================
struct VECTOR4
{
public:
	VECTOR4(){};
	VECTOR4(float fx,float fy,float fz,float fw);
	VECTOR4 operator + (const VECTOR4&) const;
	VECTOR4 operator - (const VECTOR4&) const;
	VECTOR4 operator * (const VECTOR4&) const;
	VECTOR4 operator * (const float) const;
	VECTOR4 operator / (const VECTOR4&) const;
	VECTOR4 operator / (const float) const;
	VECTOR4 operator += (const VECTOR4&);
	VECTOR4 operator -= (const VECTOR4&);
	VECTOR4 operator *= (const VECTOR4&);
	VECTOR4 operator *= (const float);
	VECTOR4 operator /= (const VECTOR4&);
	VECTOR4 operator /= (const float);

	union
	{
		struct
		{
			float x,y,z,w;
		};
		float f[4];
	};
};
//===================================================================
//RGBA
//===================================================================
typedef struct COLOR
{
public:
	COLOR(){};
	COLOR(float fr,float fg,float fb,float fa);
	COLOR operator + (const COLOR&) const;
	COLOR operator - (const COLOR&) const;
	COLOR operator * (const COLOR&) const;
	COLOR operator * (const float) const;
	COLOR operator / (const COLOR&) const;
	COLOR operator / (const float) const;
	COLOR operator += (const COLOR&);
	COLOR operator -= (const COLOR&);
	COLOR operator *= (const COLOR&);
	COLOR operator *= (const float);
	COLOR operator /= (const COLOR&);
	COLOR operator /= (const float);
	float r,g,b,a;
}COLOR;

//色を簡単に作れるテンプレート
#define WHITE(Alpha)	(COLOR(1.0f,1.0f,1.0f,Alpha))
#define RED(Alpha)		(COLOR(1.0f,0.0f,0.0f,Alpha))
#define BLUE(Alpha)		(COLOR(0.0f,0.0f,1.0f,Alpha))
#define GREEN(Alpha)	(COLOR(0.0f,1.0f,0.0f,Alpha))
#define YELLOW(Alpha)	(COLOR(1.0f,1.0f,0.0f,Alpha))
#define PURPLE(Alpha)	(COLOR(1.0f,0.0f,1.0f,Alpha))
#define CYAN(Alpha)		(COLOR(0.0f,1.0f,1.0f,Alpha))
#define BLACK(Alpha)	(COLOR(0.0f,0.0f,0.0f,Alpha))
#define GRAY(Alpha)		(COLOR(0.5f,0.5f,0.5f,Alpha))

//===================================================================
//マトリックス
//===================================================================
typedef struct FLOAT3
{
	float x,y,z;
}FLOAT3;
typedef struct FLOAT4
{
	float x,y,z,w;
}FLOAT4;

typedef struct MATRIX3x3
{
	union
	{
		struct
		{
			float	_11,_12,_13;
			float	_21,_22,_23;
			float	_31,_32,_33;
		};
		float	f[12];
		float	m[3][3];
		FLOAT4	v[3];
	};
} MATRIX3x3;

typedef struct MATRIX4x4
{
	union
	{
		struct
		{
			float	_11,_12,_13,_14;
			float	_21,_22,_23,_24;
			float	_31,_32,_33,_34;
			float	_41,_42,_43,_44;
		};
		float	f[16];
		float	m[4][4];
		FLOAT4	v[4];
	};
} MATRIX4x4;
//===================================================================
//逆行列作成
//===================================================================
inline void glMatrixInverse(MATRIX4x4 *out,const MATRIX4x4& in)
{
	out->_11 = in._11;
	out->_12 = in._21;
	out->_13 = in._31;
	out->_21 = in._12;
	out->_22 = in._22;
	out->_23 = in._32;
	out->_31 = in._31;
	out->_32 = in._23;
	out->_33 = in._33;

	out->_41 = 0;
	out->_42 = 0;
	out->_43 = 0;
	out->_14 = 0;
	out->_24 = 0;
	out->_34 = 0;
	out->_44 = 1.0f;
}

//===================================================================
//UV値
//===================================================================
typedef struct UV
{
public:
	UV(){
		x = y = 0;
		width = height = 1.0f;
		InverseH = InverseV = false;
		tex[0] = VECTOR2(x,y);
		tex[1] = VECTOR2(x+width,y);
		tex[2] = VECTOR2(x,y+height);
		tex[3] = VECTOR2(x+width,y+height);
	}
	UV(float x,float y,float width,float height)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}

	float x,y,width,height;
	bool InverseH,InverseV;
	VECTOR2 tex[4];

	//これを呼び出すと、tex配列に値がセットされる
	void Set(void)
	{
		if (InverseH)
		{
			tex[0].x = x;
			tex[1].x = x + width;
			tex[2].x = x;
			tex[3].x = x + width;
		}
		else
		{

			tex[0].x = x + width;
			tex[1].x = x;
			tex[2].x = x + width;
			tex[3].x = x;

		}
		if (InverseV)
		{
			tex[0].y = y;
			tex[1].y = y;
			tex[2].y = y + height;
			tex[3].y = y + height;
		}
		else
		{
			tex[0].y = y + height;
			tex[1].y = y + height;
			tex[2].y = y;
			tex[3].y = y;
		}
	}
}UV;

#endif