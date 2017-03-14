#pragma once

#include "..\iostream.h"


class MapRenderer
{
private:
	CQuake3BSP* m_pBSP;
	BSPMesh* m_pFace;
	Shader* m_pShader;

	GLint m_iModelUniform;
	GLint m_iViewUniform;
	GLint m_iProjUniform;
	GLint m_iTexUniform;
	CBitset m_FacesDrawn;
	mat4 m_Model;

public:
	void RenderFace(int i);
	BSPMesh* GetMesh(int i) { return &m_pFace[i]; };
public:
	MapRenderer();
	virtual ~MapRenderer();

	void Init(const char* mapFile);

	void PreRender(mat4& view, mat4& proj);
	void Render();
	void PostRender();
};
