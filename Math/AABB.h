#pragma once
#include "..\iostream.h"


// Axis-Aligned Bounding Boxes

class AABB
{
private:
	Vec3 Min;
	Vec3 Max;
public:
	AABB() {};
	~AABB() {};


	void Set(const vector<Vec3>&);
	bool Intersect(const AABB& a);
};