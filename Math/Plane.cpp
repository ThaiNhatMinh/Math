#include "Plane.h"

Plane::Plane(float a, float b, float c, float d)
{
	m_Normal.x = a;
	m_Normal.y = b;
	m_Normal.z = c;
	m_Offset = d;
}

void Plane::Init(const Vec3& v1, const Vec3& v2, const Vec3& v3)
{
	Vec3 e1 = v1 - v2;
	Vec3 e2 = v3 - v2;
	m_Normal = Math::Normalize(Math::Cross(e1,e2));
	m_Offset = Math::Dot(m_Normal, v1);
}



bool Plane::Inside(const Vec3& v, float darius)const
{
	float d = Math::Dot(m_Normal, v) + m_Offset;
	return (d>=-darius);
}