#include <math.h>
#include "CommonGL.h"

//=============================================================================
//X,Y
//=============================================================================
VECTOR2::VECTOR2(float fx,float fy)
{
	x = fx;
	y = fy;
}

VECTOR2 VECTOR2::operator + (const VECTOR2& f) const
{
	return VECTOR2(x+f.x,y+f.y);
}
VECTOR2 VECTOR2::operator - (const VECTOR2& f) const
{
	return VECTOR2(x-f.x,y-f.y);
}
VECTOR2 VECTOR2::operator * (const VECTOR2& f) const
{
	return VECTOR2(x*f.x,y*f.y);
}
VECTOR2 VECTOR2::operator * (const float f) const
{
	return VECTOR2(x*f,y*f);
}
VECTOR2 VECTOR2::operator / (const VECTOR2& f) const
{
	return VECTOR2(x/f.x,y/f.y);
}
VECTOR2 VECTOR2::operator / (const float f) const
{
	return VECTOR2(x/f,y/f);
}
VECTOR2 VECTOR2::operator += (const VECTOR2& f)
{
	x += f.x;
	y += f.y;
	
	return *this;
}
VECTOR2 VECTOR2::operator -= (const VECTOR2& f)
{
	x -= f.x;
	y -= f.y;
	return *this;
}

VECTOR2 VECTOR2::operator *= (const VECTOR2& f)
{
	x *= f.x;
	y *= f.y;
	return *this;
}

VECTOR2 VECTOR2::operator *= (const float f)
{
	x *= f;
	y *= f;
	return *this;
}

VECTOR2 VECTOR2::operator /= (const VECTOR2& f)
{
	x /= f.x;
	y /= f.y;
	return *this;
}

VECTOR2 VECTOR2::operator /= (const float f)
{
	x /= f;
	y /= f;
	return *this;
}

float VECTOR2::Dot(const VECTOR2& Src1,const VECTOR2& Src2)
{
	return (Src1.x*Src2.x + Src1.y*Src2.y);
}

float VECTOR2::Cross(const VECTOR2& Src1,const VECTOR2& Src2)
{
	return Src1.x*Src2.y - Src1.y*Src2.x;
}
//=============================================================================
//X,Y,Z
//=============================================================================
VECTOR3::VECTOR3(float fx,float fy,float fz)
{
	x = fx;
	y = fy;
	z = fz;
}

VECTOR3 VECTOR3::operator + (const VECTOR3& f) const
{
	return VECTOR3(x+f.x,y+f.y,z+f.z);
}
VECTOR3 VECTOR3::operator - (const VECTOR3& f) const
{
	return VECTOR3(x-f.x,y-f.y,z-f.z);
}
VECTOR3 VECTOR3::operator * (const VECTOR3& f) const
{
	return VECTOR3(x*f.x,y*f.y,z*f.z);
}
VECTOR3 VECTOR3::operator * (const float f) const
{
	return VECTOR3(x*f,y*f,z*f);
}
VECTOR3 VECTOR3::operator / (const VECTOR3& f) const
{
	return VECTOR3(x/f.x,y/f.y,z/f.z);
}

VECTOR3 VECTOR3::operator / (const float f) const
{
	return VECTOR3(x / f,y / f,z / f);
}

VECTOR3 VECTOR3::operator += (const VECTOR3& f)
{
	x += f.x;
	y += f.y;
	z += f.z;
	return *this;
}
VECTOR3 VECTOR3::operator -= (const VECTOR3& f)
{
	x -= f.x;
	y -= f.y;
	z -= f.z;
	return *this;
}
VECTOR3 VECTOR3::operator *= (float f)
{
	x *= f;
	y *= f;
	z *= f;
	return *this;
}
VECTOR3 VECTOR3::operator /= (float f)
{
	x /= f;
	y /= f;
	z /= f;
	return *this;
}

VECTOR3 VECTOR3::operator *= (const VECTOR3& f)
{
	x *= f.x;
	y *= f.y;
	z *= f.z;
	return *this;
}
VECTOR3 VECTOR3::operator /= (const VECTOR3& f)
{
	x /= f.x;
	y /= f.y;
	z /= f.z;
	return *this;
}

void VECTOR3::Normalize(void)
{
	float len = sqrt(x*x+y*y+z*z);
	len = 1.0f/len;

	*this *= len;
}
float VECTOR3::Dot(const VECTOR3& Src1,const VECTOR3& Src2)
{
	return (Src1.x*Src2.x + Src1.y*Src2.y + Src1.z*Src2.z);
}
void VECTOR3::Cross(VECTOR3* Output,const VECTOR3& Src1,const VECTOR3& Src2)
{
	Output->x = Src1.z*Src2.y - Src1.y*Src2.z;
	Output->y = Src1.z*Src2.x - Src1.x*Src2.z;
	Output->z = Src1.x*Src2.y - Src1.y*Src2.x;
}
//=============================================================================
//X,Y,Z,W
//=============================================================================
VECTOR4::VECTOR4(float fx,float fy,float fz,float fw)
{
	x = fx;
	y = fy;
	z = fz;
	w = fw;
}

VECTOR4 VECTOR4::operator + (const VECTOR4& f) const
{
	return VECTOR4(x+f.x,y+f.y,z+f.z,w+f.w);
}
VECTOR4 VECTOR4::operator - (const VECTOR4& f) const
{
	return VECTOR4(x-f.x,y-f.y,z-f.z,w-f.w);
}

VECTOR4 VECTOR4::operator * (const VECTOR4& f) const
{
	return VECTOR4(x*f.x,y*f.y,z*f.z,w*f.w);
}

VECTOR4 VECTOR4::operator * (const float f) const
{
	return VECTOR4(x*f,y*f,z*f,w*f);
}

VECTOR4 VECTOR4::operator / (const VECTOR4& f) const
{
	return VECTOR4(x/f.x,y/f.y,z/f.z,w/f.w);
}

VECTOR4 VECTOR4::operator / (const float f) const
{
	return VECTOR4(x/f,y/f,z/f,w/f);
}

VECTOR4 VECTOR4::operator += (const VECTOR4& f)
{
	x += f.x;
	y += f.y;
	z += f.z;
	w += f.w;
	return *this;
}
VECTOR4 VECTOR4::operator -= (const VECTOR4& f)
{
	x -= f.x;
	y -= f.y;
	z -= f.z;
	w -= f.w;
	return *this;
}

VECTOR4 VECTOR4::operator *= (const VECTOR4& f)
{
	x *= f.x;
	y *= f.y;
	z *= f.z;
	w *= f.w;

	return *this;
}

VECTOR4 VECTOR4::operator *= (const float f)
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;

	return *this;
}

VECTOR4 VECTOR4::operator /= (const VECTOR4& f)
{
	x /= f.x;
	y /= f.y;
	z /= f.z;
	w /= f.w;

	return *this;
}

VECTOR4 VECTOR4::operator /= (const float f)
{
	x /= f;
	y /= f;
	z /= f;
	w /= f;

	return *this;
}

//=============================================================================
//RGBA
//=============================================================================
COLOR::COLOR(float fr,float fg,float fb,float fa)
{
	r = fr;
	g = fg;
	b = fb;
	a = fa;
}

COLOR COLOR::operator + (const COLOR& f) const
{
	return COLOR(r+f.r,g+f.g,b+f.b,a+f.a);
}
COLOR COLOR::operator - (const COLOR& f) const
{
	return COLOR(r-f.r,g-f.g,b-f.b,a-f.a);
}
COLOR COLOR::operator * (const COLOR& f) const
{
	return COLOR(r*f.r,g*f.g,b*f.b,a*f.a);
}
COLOR COLOR::operator * (const float f) const
{
	return COLOR(r*f,g*f,b*f,a*f);
}

COLOR COLOR::operator / (const COLOR& f) const
{
	return COLOR(r/f.r,g/f.g,b/f.b,a/f.a);
}
COLOR COLOR::operator / (const float f) const
{
	return COLOR(r/f,g/f,b/f,a/f);
}

COLOR COLOR::operator += (const COLOR& f)
{
	r += f.r;
	g += f.g;
	b += f.b;
	a += f.a;
	return *this;
}
COLOR COLOR::operator -= (const COLOR& f)
{
	r -= f.r;
	g -= f.g;
	b -= f.b;
	a -= f.a;
	return *this;
}

COLOR COLOR::operator *= (const COLOR& f)
{
	r *= f.r;
	g *= f.g;
	b *= f.b;
	a *= f.a;

	return *this;
}

COLOR COLOR::operator *= (const float f)
{
	r *= f;
	g *= f;
	b *= f;
	a *= f;

	return *this;
}

COLOR COLOR::operator /= (const COLOR& f)
{
	r /= f.r;
	g /= f.g;
	b /= f.b;
	a /= f.a;

	return *this;
}

COLOR COLOR::operator /= (const float f)
{
	r /= f;
	g /= f;
	b /= f;
	a /= f;

	return *this;
}