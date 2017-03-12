#pragma once
#include "..\iostream.h"
#include "SOIL.h"
class SkyBox
{
public:
	GLuint	m_iCubeTexId;
	Cube	m_Box;
	Shader	m_Shader;
public:
	SkyBox();
	~SkyBox();

	void Init();
	void LoadCubeTexture(const vector<string>& filelist);
	void Render( mat4& view,  mat4& proj);
};

