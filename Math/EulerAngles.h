#ifndef __EULERANGLE__
#define __EULERANGLE__

// This class doesn't complete. I will finish it on the future when I learn more on Math

class Vector3;
class Quaternion;
class Matrix4;

class EulerAngle
{
public:
	// In Degree
	float pitch;	// X axis 
	float yaw;		// Z axis bank
	float roll;		// Y axis heading
	EulerAngle();
	EulerAngle(float pitch, float yaw, float roll);
	EulerAngle(const Vector3& V);
	void Identity();
	void Set(float pitch, float yaw, float roll);
	EulerAngle& operator=(const EulerAngle& EA);
	Matrix4 ToMatrix();
	Quaternion ToQuaternion();
	~EulerAngle();
};
#endif

