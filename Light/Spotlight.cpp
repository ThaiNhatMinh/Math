#include "Spotlight.h"



Spotlight::Spotlight(Vec3 pos, float cutoff, float outercutoff, PointLight point)
{
	Position = pos;
	Direction = Vec3(0, 0, 0);
	Ambient = point.Ambient;
	Diffuse = point.Diffuse;
	Specular = point.Specular;
	fConstant = point.fConstant;
	fLinear = point.fLinear;
	fQuadratic = point.fQuadratic;
	cutOff = cutoff;
	OutercutOff = outercutoff;

}

Spotlight::~Spotlight()
{
}

void Spotlight::SendData(Shader * pShader)
{
	pShader->SetUniform("spotlight.position", Position);
	pShader->SetUniform("spotlight.ambient", Ambient);
	pShader->SetUniform("spotlight.diffuse", Diffuse);
	pShader->SetUniform("spotlight.specular", Specular);
	pShader->SetUniform("spotlight.constant", fConstant);
	pShader->SetUniform("spotlight.linear", fLinear);
	pShader->SetUniform("spotlight.quadratic", fQuadratic);
	pShader->SetUniform("spotlight.direction", Direction);
	pShader->SetUniform("spotlight.cutOff", cutOff);
	pShader->SetUniform("spotlight.outercutOff", OutercutOff);
}
