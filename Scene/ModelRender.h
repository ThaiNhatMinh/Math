#pragma once
#include "..\iostream.h"

// Skeleton enity
class SkeEnity : public SceneNode
{
protected:
	Shader* m_pShader;
	GLuint m_iViewUniform;
	GLuint m_iProjUniform;
	GLuint m_iModelUniform;
	GLuint m_iLightPosUniform;
	GLuint m_iEyePosUniform;
	GLuint m_numPointlightUniform;
	GLuint m_numSpotlightUniform;
	GLuint m_iTransformUniform[60];
	GLuint m_iInvPoseUniform[60];
	JointList jointlist;
	LTBProp				*m_pProp;
	vector<LTBMesh*>	m_pMeshs;
	vector<SkeNode*>	m_pNodeLists;
	BaseAnim*			m_pAnimation;
	vector<string>		m_ChildModel;
	vector<LTBSocket>	m_Socket;
	//vector<WeightSet>	m_WS;
	float m_fSpeed;
	//MotionRegister*		m_pPlayerMotion;
	//vector<SocketItem*> m_pSocket;
	

	
protected:
	void RenderMesh(LTBMesh* mesh);
	void LoadModel(const char* FileName);
	virtual void LoadTexture(const char* FileName) = 0;
public:
	SkeEnity();
	~SkeEnity();

	bool Init(Scene*, const char* FileName);
	virtual void SetAnimation(int) =0;
	virtual void NextAnim() = 0;
	virtual void AddAnimation(int) = 0;
	//void OnUpdate(Scene*, float);

	// this function must be call first in deliver class
	void PreRender(Scene*);
	
	// render model
	void Render(Scene*);
	void SetRotate(float angle);
	// some restore attitude
	//void PostRender(Scene*);
	float GetSpeed() { return m_fSpeed; }
};