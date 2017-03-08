#ifndef __FRUSTUM__
#define __FRUSTUM__
#include "Plane.h"
#include "Vector.h"


Vec3 gRight		(1,0,0);
Vec3 gUp		(0,1,0);
Vec3 gForward	(0,0,1);
class Frustum
{
private:
	enum Side{Near, Far, Top, Bottom, Left, Right, NumPlane};

public:
	Plane m_Plane[NumPlane];			// Plane of frustum in camera space
	Vec3 m_NearClip[4];					// 4 point in near clip plane in camera space
	Vec3 m_FarClip[4];					// 4 point in far clip plane in camera space
	
	float m_Fov;						// fied of view in radian
	float m_Aspect;						// aspect ratio  - width / height
	float m_Near;						// Near distance
	float m_Far;						// Far distance

public:
	Frustum();
	~Frustum();
	void Init(float fov, float aspect, float _near, float _far);
	bool Inside(const Vec3& v)const;
	bool Inside(const Vec3& v, float radius)const;
	const Plane& Get(Side side){return m_Plane[side];}
	void SetFov(float fov){ m_Fov = fov; Init(m_Fov, m_Aspect, m_Near, m_Far);}
	void SetAspect(float aspect) { m_Aspect = aspect;  Init(m_Fov, m_Aspect, m_Near, m_Far); };
	void SetNear(float _near) { m_Near = _near;  Init(m_Fov, m_Aspect, m_Near, m_Far); };
	void SetFar(float _far) { m_Far = _far;  Init(m_Fov, m_Aspect, m_Near, m_Far); };

	void Render();
};
	
#endif