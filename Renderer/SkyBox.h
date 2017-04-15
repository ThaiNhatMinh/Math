#pragma once
#include "..\iostream.h"

class SkyBox:public SceneNode
{
public:
	//GLuint	m_iCubeTexId;
	Texture* m_pCubeTex;
	Cube	m_Box;
	Shader	m_Shader;
	Quat	m_Ort;
	float	m_fAngle;
public:
	SkyBox();
	~SkyBox();

	void Init();
	//void LoadCubeTexture(const vector<string>& filelist);
	void OnUpdate(Scene*, float);
	void PreRender(Scene*);
	void Render(Scene*);
	void PostRender(Scene*);
};

