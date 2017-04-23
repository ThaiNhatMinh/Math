#pragma once
#include "..\iostream.h"

// third person camera
class SkeEnity;
class TPSCamera : public Camera
{
private:
	float m_fDistance;			// Distance to target
	Vec3 m_TargetPos;			//
	SkeEnity* m_pTarget;		// 
	float m_fPitchPlayer;
	float m_fYawPlayer;

public:
	TPSCamera();
	~TPSCamera() {};

	void Update(float dt);
	virtual void OnMouse(float dx, float dy);

	void SetTarget(SkeEnity* pTarget);
	void SetDistance(float distance);
};