#pragma once
#include "..\iostream.h"
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
	GLuint slot;
public:
	PointLight(Vec3 pos, Vec3 amb, Vec3 diff, Vec3 spec) :Position(pos), Ambient(amb), Diffuse(diff), Specular(spec) {};
	~PointLight();

	void SetAttenuation(float constant, float linear, float quadratic) { fConstant = constant, fLinear = linear, fQuadratic = quadratic; };
	
	void SendData(Shader* pShader);
	void Update(float dt);
};

