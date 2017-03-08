#include "pfgen.h"

void ParticleForceRegistry::updateForces(float deltaTime)
{
	Registry::iterator i = registrations.begin();

	for(; i<registrations.end(); i++)

		i->fg->updateForce(i->particle, deltaTime);

}


// Gravity Force

void GravityForce::updateForce(Particle* particle, float deltaTime)
{
	if(!particle->hasFiniteMass()) return;
	particle->AddForce(m_vGravity * particle->getMass());
}


// Drag Force

void DragForce::updateForce(Particle* particle, float deltaTime)
{
	Vec3 force;
	particle->getVelocity(force);

	// Calculate the total drag coefficient.
	float dragCoeff = force.Length();
	dragCoeff = k1 * dragCoeff + k2 * dragCoeff * dragCoeff;

	force.Normalize();
	force *= -dragCoeff;
	particle->AddForce(force);
}

// Spring force

void SpringForce::updateForce(Particle* particle, float deltaTime)
{
	Vector3 force;
	particle->getPosition(force);

	force -= m_pOther->getPosition();
	float len = force.Length();
	len = Math::FAbs(len - m_fRestLength);
	len *=	m_fSpringConst;
	force.Normalize();
	force *=-len;
	particle->AddForce(force);
}


// Anchor Spring force

void AnchoredSpringForce::updateForce(Particle* particle, float deltaTime)
{
	Vector3 force;
	particle->getPosition(force);

	force -= *m_vAnchor;
	float len = force.Length();
	len = Math::FAbs(len - m_fRestLength);
	len *=	m_fSpringConst;
	force.Normalize();
	force *=-len;
	particle->AddForce(force);
}

void BungeeForce::updateForce(Particle* particle, float deltaTime)
{
	Vector3 force;
	particle->getPosition(force);

	force -= m_pOther->getPosition();
	float len = force.Length();
	if(len < m_fRestLength) return;
	len =	m_fSpringConst*(-len + m_fRestLength);
	force.Normalize();
	force *=-len;
	particle->AddForce(force);
}