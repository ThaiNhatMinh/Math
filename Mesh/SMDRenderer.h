#pragma once
#include "..\iostream.h"


class SMDRenderer
{
private:
	Shader* m_pShader;
	GLuint m_iViewUniform;
	GLuint m_iProjUniform;
	GLuint m_iModelUniform;
	GLuint m_iTI_ModelUniform;
	GLuint m_iLightPosUniform;
	GLuint m_iEyePosUniform;
	GLuint m_iTransformUniform[60];
	SMDFile m_SMD;
	vector<SMDAnimation> m_AnimationList;
	mat4 m_Model;
protected:
	void RenderMesh(SMDMesh* mesh);
public:
	SMDRenderer();
	~SMDRenderer();

	bool Init(const char* FileName);
	
	void Update(float dt) {};
	void PreRender(mat4& view, mat4& proj,Camera&);
	void Render();
	void PostRender();

};