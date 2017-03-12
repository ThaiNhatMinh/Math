#include "Particle.h"
#include <assert.h>

void Particle::integrate(float deltaTime)
{
	assert(deltaTime>0.0);

	// x = x0 + v*t
	m_vPosition += m_vVelocity*deltaTime;

	Vec3 sumAcc = m_vAcceleration;
	//  a = F/m
	sumAcc += m_vForceAccum * m_fInverseMass;

	// v = v0 + a*t
	m_vVelocity += sumAcc*deltaTime;

	m_vVelocity *= Math::Pow(m_fDamping,deltaTime);

	clearForce();

}


void Particle::clearForce()
{
	m_vForceAccum.zero();
}

void Particle::AddForce(const Vec3& force)
{
	m_vForceAccum +=force;
}
void Particle::setMass(float mass)
{
	m_fInverseMass = 1.0f / mass;
}

void Particle::setInverseMass(float InvMass)
{
	m_fInverseMass = InvMass;
}

bool Particle::hasFiniteMass()const
{
	return m_fInverseMass>=0.0f;
}

float Particle::getMass()const
{
	if(m_fInverseMass==0.0f) return FLT_MAX;
	return 1.0f/ m_fInverseMass;
}

void Particle::getVelocity(Vec3& v)const
{
	v = m_vVelocity;
}
Vec3 Particle::getVelocity()const
{
	return m_vVelocity;
}


void Particle::getPosition(Vec3& v)const
{
	v = m_vPosition;
}
Vec3 Particle::getPosition()const
{
	return m_vPosition;
}