#pragma once
#include "iostream.h"
class DirectionLight :	public ILight
{
public:

	Vec3 Direction;
	Vec3 Ambient;
	Vec3 Diffuse;
	Vec3 Specular;

public:
	DirectionLight(Vec3 dir, Vec3 amb, Vec3 diff, Vec3 spec) :Direction(dir), Ambient(amb), Diffuse(diff), Specular(spec) {};
	~DirectionLight();

	void SendData(Shader* pShader);
	void Update(float dt);
};

