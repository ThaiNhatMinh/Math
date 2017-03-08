
#include "Math\CMath.h"
#include "Math\Plane.h"

#include "iostream.h"

void main_plane()
{
	Plane plane(0.0f, 1.0f, 0.0f, 5.0f);

	if (plane.Inside(Vec3(0, 5, 0))) cout << " Inside!" << endl;
	cout << plane.DistanceToPlane(Vec3(0, 10, 0)) << endl;
	system("pause");
}