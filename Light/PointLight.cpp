#include "..\iostream.h"



PointLight::~PointLight()
{
}

void PointLight::SendData(Shader * pShader)
{
	char buffer[4];
	_itoa(slot, buffer, 10);
	string s = buffer;
	pShader->SetUniform(("pointlight[" + s + "].position").c_str(), Position);
	pShader->SetUniform(("pointlight[" + s + "].ambient").c_str(), Ambient);
	pShader->SetUniform(("pointlight[" + s + "].diffuse").c_str(), Diffuse);
	pShader->SetUniform(("pointlight[" + s + "].specular").c_str(), Specular);
	pShader->SetUniform(("pointlight[" + s + "].constant").c_str(), fConstant);
	pShader->SetUniform(("pointlight[" + s + "].linear").c_str(), fLinear);
	pShader->SetUniform(("pointlight[" + s + "].quadratic").c_str(), fQuadratic);
}

void PointLight::Update(float dt)
{
	// nothing here
}
