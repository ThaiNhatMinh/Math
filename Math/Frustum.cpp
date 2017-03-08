#include "Frustum.h"
#include "CMath.h"

Frustum::Frustum()
{
	m_Fov = Math::PI/ 4.0f; // default is 90
	m_Aspect  = 4.0f / 3.0f;
	m_Near = 1.0f;
	m_Far = 1000.0f;

}

Frustum::~Frustum()
{
}

bool Frustum::Inside(const Vec3& v)const
{
	for(int i=0; i<NumPlane; i++)
		if(!m_Plane[i].Inside(v)) return false;
	return true;
}

bool Frustum::Inside(const Vec3& v, float radius)const
{
	for(int i=0; i<NumPlane; i++)
		if(!m_Plane[i].Inside(v,radius)) return false;

	return true;
}

void Frustum::Render()
{
}

void Frustum::Init(float fov, float aspect, float near, float far)
{
	m_Fov = fov;
	m_Aspect = aspect;
	m_Near = near;
	m_Far = far;

	double tanFovOver2 = Math::Tan(m_Fov/2);
	Vec3 nearRight =  gRight * ((m_Near * tanFovOver2 ) * m_Aspect);
	Vec3 farRight =  gRight * ((m_Far * tanFovOver2 ) * m_Aspect);
	Vec3 nearUp =  gUp * (m_Near * tanFovOver2 ) ;
	Vec3 farUp =  gUp * (m_Far * tanFovOver2 ) ;

	m_NearClip[0] = (gForward  * m_Near) - nearRight + nearUp;
	m_NearClip[1] = (gForward  * m_Near) + nearRight + nearUp;
	m_NearClip[2] = (gForward  * m_Near) + nearRight - nearUp;
	m_NearClip[3] = (gForward  * m_Near) - nearRight - nearUp;

	m_FarClip[0] = (gForward  * m_Far) - farRight + farUp;
	m_FarClip[1] = (gForward  * m_Far) + farRight + farUp;
	m_FarClip[2] = (gForward  * m_Far) + farRight - farUp;
	m_FarClip[3] = (gForward  * m_Far) - farRight - farUp;


	Vec3 Origin(0,0,0);
	m_Plane[Near].Init(m_NearClip[2],m_NearClip[1],m_NearClip[0]);
	m_Plane[Far].Init(m_FarClip[0],m_FarClip[1],m_FarClip[2]);
	m_Plane[Right].Init(m_FarClip[2],m_FarClip[1],Origin);
	m_Plane[Left].Init(m_FarClip[0],m_FarClip[3],Origin);
	m_Plane[Top].Init(m_FarClip[1],m_FarClip[0],Origin);
	m_Plane[Bottom].Init(m_FarClip[3],m_FarClip[2],Origin);
}