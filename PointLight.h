#pragma once
#include "iostream.h"
class PointLight :	public ILight
{
public:
	Vec3 Position;
	Vec3 Ambient;
	Vec3 Diffuse;
	Vec3 Specular;
	float fConstant;
	float fLinear;
	float fQuadratic;

public:
	PointLight(Vec3 pos, Vec3 amb, Vec3 diff, Vec3 spec) :Position(pos), Ambient(amb), Diffuse(diff), Specular(spec) {};
	~PointLight();

	void SetAttenuation(float constant, float linear, float quadratic) { fConstant = constant, fLinear = linear, fQuadratic = quadratic; };
	
	void SendData(Shader* pShader,int slot);
	void Update(float dt);
protected:
	void SendData(Shader* pShader) {};
};

