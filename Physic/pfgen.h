#include "./Particle.h"
#include <vector>

#ifndef __FORCEGEN__
#define __FORCEGEN__
class IForceGenerator
{
public:
	virtual void updateForce(Particle* particle, float deltaTime) = 0;
};


/**
* Holds all the force generators and the particles they apply to.
*/
class ParticleForceRegistry
{
protected:
	struct ParticleForceRegistration
	{
		Particle* particle;
		IForceGenerator* fg;
	};

	// Hold the list of registration
	typedef std::vector<ParticleForceRegistration> Registry;
	Registry registrations; 
public:

	void add(Particle* particle, IForceGenerator* fg);
	void remove(Particle* particle, IForceGenerator* fg);
	void clear();

	/**
	* Calls all the force generators to update the forces of
	* their corresponding particles.
	*/
	void updateForces(float deltaTime);
};


class GravityForce: public IForceGenerator
{
	Vec3 m_vGravity;
public:
	GravityForce(const Vector3& gravity):m_vGravity(gravity){};
	virtual void updateForce(Particle* particle, float deltaTime);
};

class DragForce: public IForceGenerator
{
	// hold velocity drag coefficient
	float k1;
	// hold vellcity square drag coefficient
	float k2;
public:
	DragForce(float k1, float k2):k1(k1),k2(k2){};
	virtual void updateForce(Particle* particle, float deltaTime);
};


class SpringForce: public IForceGenerator
{
	Particle* m_pOther;
	float m_fSpringConst;
	float m_fRestLength;
public:
	SpringForce(Particle* other, float springconst, float restlength):m_pOther(other),m_fSpringConst(springconst),m_fRestLength(restlength){};
	virtual void updateForce(Particle* particle, float deltaTime );
};

class AnchoredSpringForce: public IForceGenerator
{
	Vec3* m_vAnchor;
	float m_fSpringConst;
	float m_fRestLength;
public:
	AnchoredSpringForce(Vec3* anchor, float springconst, float restlength):m_vAnchor(anchor),m_fSpringConst(springconst),m_fRestLength(restlength){};
	virtual void updateForce(Particle* particle, float deltaTime );
};

class BungeeForce: public IForceGenerator
{
	Particle* m_pOther;
	float m_fSpringConst;
	float m_fRestLength;
public:
	BungeeForce(Particle* other, float springconst, float restlength):m_pOther(other),m_fSpringConst(springconst),m_fRestLength(restlength){};
	virtual void updateForce(Particle* particle, float deltaTime );
};

#endif