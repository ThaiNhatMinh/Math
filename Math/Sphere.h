#pragma once
#include "..\iostream.h"


// Bounding sphere
class BVSphere
{
private:
	Vec3 m_Pos;
	float m_fRadius;

public:
	BVSphere() ;
	~BVSphere() {};


	float GetRadius() const{ return m_fRadius; }
	Vec3 GetPos()const { return m_Pos; }
	float& GetRadius() { return m_fRadius; }
	Vec3& GetPos() { return m_Pos; }

	void Set(const vector<Vec3>& v);

	// sphere
	bool Intersect(const BVSphere& s);

	// ray
	bool Intersect(const Vec3& pos, const Vec3& direction);
	// plane
	float Classify(const Plane& p);

};