#pragma once
#include "iostream.h"
class Spotlight :	public ILight
{
public:
	Vec3 Position;
	Vec3 Direction;
	Vec3 Ambient;
	Vec3 Diffuse;
	Vec3 Specular;
	float fConstant;
	float fLinear;
	float fQuadratic;
	float cutOff;
	float OutercutOff;
public:
	Spotlight(Vec3 pos, float cutoff,float outercutoff, PointLight point);
	~Spotlight();

	void SendData(Shader* pShader);
	void Update(float dt) {};
};

