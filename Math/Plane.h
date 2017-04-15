
#pragma once

#include "Vector.h"
#include "CMath.h"
// ax + by + cz = d
// Normal is (a,b,c)
// Offset is d
class Plane
{
public:
	Vec3 m_Normal;
	float m_Offset;
	Plane(){};
	Plane(float a, float b, float c, float d);
	Plane(Vec3 normal, float offset) :m_Normal(normal), m_Offset(offset) {}
	// tree point in counter clockwise order
	void Init(const Vec3& v1, const Vec3& v2, const Vec3& v3);


	bool Inside(const Vec3& v, float darius) const;
	bool inline Inside(const Vec3& v)const {
		return (Math::Dot(m_Normal, v) == m_Offset);
	};
	float inline DistanceToPlane(const Vec3& v)const {
		float result = Math::Dot(m_Normal, v) - m_Offset;
		return result;
	}
	//bool IntersectionLine(const Vec3& v1, const Vec3& v2) const;

};