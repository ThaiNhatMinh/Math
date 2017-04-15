
#include "Math\CMath.h"
#include "Math\Plane.h"

#include "..\iostream.h"

void main__()
{
	Plane plane(0.0f, 1.0f, 0.0f, 5.0f);
	plane.Init(Vec3(1, 0, 0), Vec3(0, 0, 1), Vec3(0, 0, 0));
	
	cout << plane.DistanceToPlane(Vec3(0, 10, 0)) << endl;
	cout << plane.m_Normal << endl;
	system("pause");
}