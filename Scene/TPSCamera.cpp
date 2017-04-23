#include "..\iostream.h"

TPSCamera::TPSCamera()
{
	m_fDistance = 500.0f;
	m_fPitchPlayer = 0.0f;
	m_fYawPlayer = 0.0f;
}
void TPSCamera::Update(float dt)
{
	// update target control
	if(gInput.Press(GLFW_KEY_R)) m_pTarget->AddAnimation(sniper + reload);
	else if(gInput.OnMouse(GLFW_MOUSE_BUTTON_LEFT)) m_pTarget->AddAnimation(sniper + shoot);

	//return;
	// update target movement
	Vec3 pos = m_pTarget->GetPosition();
	Vec3 front(m_Front.x, 0, m_Front.z);
	Vec3 right(m_Right.x, 0, m_Right.z);
	float f = m_pTarget->GetSpeed();
	if (gInput.OnKey(GLFW_KEY_LEFT_SHIFT)) f += 200;
	if (gInput.OnKey(GLFW_KEY_W))
	{
		pos += front*f*dt;
		if (gInput.OnKey(GLFW_KEY_LEFT_SHIFT)) m_pTarget->SetAnimation(sniper + run);
		else m_pTarget->SetAnimation(sniper + walk);
	}
	else if (gInput.OnKey(GLFW_KEY_S))
	{
		pos -= front*f*dt;
		if (gInput.OnKey(GLFW_KEY_LEFT_SHIFT)) m_pTarget->SetAnimation(sniper + runBside);
		else m_pTarget->SetAnimation(sniper + runBside);
	}
	else if (gInput.OnKey(GLFW_KEY_D))
	{
		pos += right*f*dt;
		if (gInput.OnKey(GLFW_KEY_LEFT_SHIFT)) m_pTarget->SetAnimation(sniper + runRside);
		else m_pTarget->SetAnimation(sniper + runRside);
	}
	else if (gInput.OnKey(GLFW_KEY_A))
	{
		pos -= right*f*dt;
		if (gInput.OnKey(GLFW_KEY_LEFT_SHIFT)) m_pTarget->SetAnimation(sniper + runLside);
		else m_pTarget->SetAnimation(sniper + runLside);
	}
	else if (gInput.Press(GLFW_KEY_SPACE))
	{
		m_pTarget->SetAnimation(jump);
	}
	else m_pTarget->SetAnimation(sniper + idle);
	pos.y = 100;
	m_pTarget->SetPosition(pos);
	OnMouse(0, 0);



}

void TPSCamera::OnMouse(float dx, float dy)
{
	m_fPitchPlayer += dy*MouseSensitivity;
	m_fYawPlayer += dx*MouseSensitivity;
	
	if (m_fPitchPlayer >= 45.0f) m_fPitchPlayer = 45.0f;
	if (m_fPitchPlayer <= -45.0f) m_fPitchPlayer = -45.0f;
	m_Position.x = cos(Math::ToRadian(m_fYawPlayer)) * cos(Math::ToRadian(m_fPitchPlayer))* m_fDistance;
	m_Position.y = sin(Math::ToRadian(m_fPitchPlayer))* m_fDistance;
	m_Position.z = sin(Math::ToRadian(m_fYawPlayer)) * cos(Math::ToRadian(m_fPitchPlayer))* m_fDistance;

	//m_Position.x = Math::Cos(Math::ToRadian(m_fYawPlayer)) * m_fDistance;
	//m_Position.z = Math::Sin(Math::ToRadian(m_fYawPlayer)) * m_fDistance;
	m_Position += m_pTarget->GetPosition();
	//m_Position.y = 100;
	m_Front = Math::Normalize(m_pTarget->GetPosition() - m_Position);
	m_Right = Math::Normalize(Math::Cross(m_Front, WorldUp));
	m_Up = Math::Normalize(Math::Cross(m_Right, m_Front));
	
	m_pTarget->SetRotate(90.0f + m_fYawPlayer);
}

void TPSCamera::SetTarget(SkeEnity * pTarget)
{
	m_pTarget = pTarget;
}

void TPSCamera::SetDistance(float distance)
{
	m_fDistance = distance;
}
