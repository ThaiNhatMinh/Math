#include "..\Math\CMath.h"
#include "..\Math\Vector.h"
#ifndef __PARTICLE__
#define __PARTICLE__
class Particle
{
public:
	// member data
	Vector3 m_vPosition;
	Vector3 m_vVelocity;
	Vector3 m_vAcceleration;
	Vector3 m_vForceAccum;
	float m_fDamping;
	float m_fInverseMass;

public:
	void integrate(float deltaTime);
	void setMass(float mass);
	void setInverseMass(float InvMass);
	void clearForce();
	void AddForce(const Vec3& force);

	bool hasFiniteMass()const;
	float getMass()const;
	void getVelocity(Vec3& v)const;
	Vec3 getVelocity()const;
	void getPosition(Vec3& v) const;
	Vec3 getPosition()const;
};
#endif