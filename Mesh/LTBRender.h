#pragma once
#include "..\iostream.h"

class LTBRenderer
{
private:
	Shader* m_pShader;
	GLuint m_iViewUniform;
	GLuint m_iProjUniform;
	GLuint m_iModelUniform;
	GLuint m_iLightPosUniform;
	GLuint m_iEyePosUniform;
	GLuint m_iTransformUniform[60];
	GLuint m_iInvPoseUniform[60];
	LTBFile m_pLTB;
	mat4 m_Model;
	JointList jointlist;
	//SkeletonNode* m_pSkeleton;
	vector<LTBMesh*> m_pMeshList;
	GLuint numNode;


	
protected:
	void RenderMesh(LTBMesh* mesh);
public:
	LTBRenderer();
	~LTBRenderer();

	bool Init(const char* FileName);

	void SetAnimation(int);
	void NextAnim();
	void SetTransform(mat4 transform) ;
	void Update(float dt);
	void PreRender(mat4& view, mat4& proj, Camera&);
	void Render();
	void PostRender();

};