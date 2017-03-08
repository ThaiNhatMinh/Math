#include "DirectionLight.h"



DirectionLight::~DirectionLight()
{
}

void DirectionLight::SendData(Shader * pShader)
{
	pShader->SetUniform("dirlight.direction", Direction);
	pShader->SetUniform("dirlight.ambient", Ambient);
	pShader->SetUniform("dirlight.diffuse", Diffuse);
	pShader->SetUniform("dirlight.specular", Specular);
}

void DirectionLight::Update(float dt)
{
	// nothing here
}
