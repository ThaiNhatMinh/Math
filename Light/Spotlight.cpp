#include "..\iostream.h"



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
	char buffer[4];
	_itoa(slot, buffer, 10);
	string s = buffer;

	pShader->SetUniform(("spotlight[" + s + "].position").c_str(), Position);
	pShader->SetUniform(("spotlight[" + s + "].ambient").c_str(), Ambient);
	pShader->SetUniform(("spotlight[" + s + "].diffuse").c_str(), Diffuse);
	pShader->SetUniform(("spotlight[" + s + "].specular").c_str(), Specular);
	pShader->SetUniform(("spotlight[" + s + "].constant").c_str(), fConstant);
	pShader->SetUniform(("spotlight[" + s + "].linear").c_str(), fLinear);
	pShader->SetUniform(("spotlight[" + s + "].quadratic").c_str(), fQuadratic);
	pShader->SetUniform(("spotlight[" + s + "].direction").c_str(), Direction);
	pShader->SetUniform(("spotlight[" + s + "].cutOff").c_str(), cutOff);
	pShader->SetUniform(("spotlight[" + s + "].outercutOff").c_str(), OutercutOff);
}
