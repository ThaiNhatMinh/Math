#pragma once
#include "..\iostream.h"
class Camera
{
	friend class Frustum;
	friend class MapRenderer;
private:
	Vec3 m_Position;
	Vec3 m_Front;
	Vec3 m_Right;
	Vec3 WorldUp;
	Vec3 m_Up;
	float m_Speed;
	float m_Pitch;	// x axis
	float m_Yaw;	// y axis
	float MouseSensitivity;

private:
	void UpdateVector();
public:
	Camera();
	
	// target and up vector must be normalize
	Camera(const Vec3& pos, const Vec3& target, const Vec3& up);
	~Camera();

	void OnKeyboard(int key,float deltaTime);
	void Update(float dt);
	void OnMouse(float dx, float dy);
	mat4 GetViewMatrix();

	inline void SetSpeed(float speed) { m_Speed = speed; };
	inline Vec3 GetFront() { return m_Front; };
	inline Vec3 GetPosition() { return m_Position; };
	inline Vec2 GetRorate() { return Vec2(m_Pitch, m_Yaw); };
	inline Vec3 GetUP() { return m_Up; };
};

