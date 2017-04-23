#include "..\iostream.h"

void CharPlayer::LoadTexture(const char * FileName)
{


	string file = FileName;
	size_t t1 = file.find_last_of("\\/");
	size_t t2 = file.size() - t1;
	string texBodyFile = file.substr(t1, t2 - 4) + ".DTX";
	string texFaceFile = file.substr(t1, t2 - 11) + "FACE_BL.DTX";
	string TexPathFile = (file.substr(0, t1)) + texBodyFile;
	string TexFaceFile = (file.substr(0, t1)) + texFaceFile;
	m_pMeshs[0]->m_pTexture = Resources::LoadDTX(TexPathFile.c_str());
	m_pMeshs[1]->m_pTexture = Resources::LoadDTX(TexFaceFile.c_str());
}

CharPlayer::CharPlayer()
{
}

CharPlayer::~CharPlayer()
{

	//delete m_pPlayerMotion;
	/* THIS CODE MOVE TO SkeEnity */
	/*
	delete m_pShader;
	delete m_pAnimation;
	for (GLuint i = 0; i < m_pNodeLists.size(); i++)
		delete m_pNodeLists[i];
	for (size_t i = 0; i < m_pMeshs.size(); i++)
		delete m_pMeshs[i];
		*/
}

bool CharPlayer::Init(Scene* pScene,const char * FileName)
{
	
	SkeEnity::Init(pScene, FileName);
	/* THIS CODE MOVE TO SkeEnity
	m_pShader = new Shader("Game\\Shader\\Skeleton.vs", "Game\\Shader\\Skeleton.frag");
	m_pShader->LinkShader();
	m_iModelUniform = m_pShader->GetUniformLocation("Model");
	m_iViewUniform = m_pShader->GetUniformLocation("View");
	m_iProjUniform = m_pShader->GetUniformLocation("Proj");
	m_iLightPosUniform = m_pShader->GetUniformLocation("lightPos");
	m_iEyePosUniform = m_pShader->GetUniformLocation("EyePos");

	

	LoadModel(FileName);
	LoadTexture(FileName);

	

	for (GLuint i = 0; i <m_pProp->m_nNodes; i++)
	{
		char buffer[4];
		_itoa(i, buffer, 10);
		string s = buffer;
		m_iTransformUniform[i] = m_pShader->GetUniformLocation(("Transform[" + s + "]").c_str());
		m_iInvPoseUniform[i] = m_pShader->GetUniformLocation(("InvBindPose[" + s + "]").c_str());
	}

	m_pPlayerMotion = new MotionRegister;
	m_pPlayerMotion->m_This = this;
	m_pPlayerMotion->m_fTime = 0;
	m_pPlayerMotion->m_iCurrentAnim = 0;
	m_pPlayerMotion->m_iCurrentFrame = 0;
	m_pPlayerMotion->m_bOnAnimation = 0;
	m_pPlayerMotion->m_fBlendTime = 0.2f;

	*/

	PlayerMotion* pm = pScene->GetMotion();
	pm->SetData(m_pAnimation);

	int num = 0;
	for (size_t i = 0; i < m_pProp->m_nSockets; i++)
	{
		if (strstr(m_Socket[i].m_Name, "righthand"))
		{
			SocketItem* p = new SocketItem;
			p->Init("Game\\Model\\WEAPONS\\AWM.LTB");
			AddChild(p);
			m_iWeaponSocket = num;
			num++;
		}
	}
	return 1;
}

void CharPlayer::SetAnimation(int a)
{
	/*if (a != m_pPlayerMotion->m_iCurrentAnim)
	{
		m_pPlayerMotion->m_iCurrentAnim = a;
		//m_pPlayerMotion->m_iCurrentFrame = 0;
		m_pPlayerMotion->m_bOnAnimation = 2;
		m_pPlayerMotion->m_fTime = 0.0f;
	}*/

	m_pAnimation->SetAnim(a);
}

void CharPlayer::AddAnimation(int a)
{
	m_pAnimation->AddAnimation(a);
}

void CharPlayer::NextAnim()
{
	//m_pPlayerMotion->m_iCurrentAnim++;
	//m_pPlayerMotion->m_bOnAnimation = 1;
	m_pAnimation->NextAnim();
}

void CharPlayer::OnUpdate(Scene* pScene,float dt)
{
	m_pAnimation->Update(dt, jointlist);
	SceneNode::OnUpdate(pScene, dt);
	
	// CPU Skinning
	/*
	for (size_t i = 0; i < m_LTB.m_Meshs.size(); i++)
	{
		LTBMesh& mesh = m_LTB.m_Meshs[i];
		for (size_t j = 0; j < mesh.m_vVertex.size(); j++)
		{
			Vec3 pos = Vec3(0);
			Vertex& v = mesh.m_vVertex[j];
			for (size_t k = 0; k < 4; k++)
			{
				Weight& w = v.Weights[k];
				if (w.weight && w.Bone<60)
				{
					pos += jointlist[w.Bone].InvBindPose * jointlist[w.Bone].LocalTransform * mesh.m_vPositions[j] * w.weight;
				}
			}
			v.Pos = pos;
		}
		glBindVertexArray(mesh.m_iVAO);
		glBindBuffer(GL_ARRAY_BUFFER, mesh.m_iVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, mesh.m_vVertex.size()*sizeof(Vertex), &mesh.m_vVertex[0]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}*/

	
}

void CharPlayer::PreRender(Scene* pScene)
{
	SkeEnity::PreRender(pScene);
	/* THIS CODE MOVE TO SkeEnity
	Camera camera = pScene->GetCamera();
	Frustum frustum = pScene->GetFrustum();
	mat4 view = camera.GetViewMatrix();
	mat4 proj = frustum.GetProjMatrix();

	m_pShader->Use();
	
	Vec3 pos = camera.GetPosition();
	//pos = pos + Vec3(0.0, 10.0, 0.0);
	glUniformMatrix4fv(m_iModelUniform, 1, GL_FALSE, m_Model.ToFloatPtr());
	glUniformMatrix4fv(m_iViewUniform, 1, GL_FALSE, view.ToFloatPtr());
	glUniformMatrix4fv(m_iProjUniform, 1, GL_FALSE, proj.ToFloatPtr());
	glUniform3f(m_iEyePosUniform, pos.x, pos.y, pos.z);
	glUniform3f(m_iLightPosUniform, -100, -100, -100);
	
	if (jointlist.size())
	{
		for (GLuint i = 0; i < m_pProp->m_nNodes; i++)
		{
			mat4 tt = m_pNodeLists[i]->m_InvBindPose * jointlist[i].LocalTransform;

			//m_pShader->SetUniformMatrix(m_iInvPoseUniform[i], jointlist[i].InvBindPose.ToFloatPtr());
			m_pShader->SetUniformMatrix(m_iTransformUniform[i], tt.ToFloatPtr());
		}
	}
	else
	{
		for (GLuint i = 0; i < m_pProp->m_nNodes; i++)
		{
			mat4 tt;// = m_pLTB.m_pNodeLists[i]->m_InvBindPose * jointlist[i].LocalTransform;

			//m_pShader->SetUniformMatrix(m_iInvPoseUniform[i], jointlist[i].InvBindPose.ToFloatPtr());
			m_pShader->SetUniformMatrix(m_iTransformUniform[i], tt.ToFloatPtr());
		}
	}
	*/
	
	for (size_t i = 0; i < m_pProp->m_nSockets; i++)
	{
		if (strstr(m_Socket[i].m_Name, "righthand"))
		{
			GLuint id = m_Socket[i].m_iNode;
			Quat ort;
			ort.setRotateXAxis(90.0f);
			mat4 transform = ort.ToMatrix();
			transform.Translate(m_Socket[i].m_Pos);
			mat4 tt = transform*jointlist[id].LocalTransform*m_Model;
			m_Children[m_iWeaponSocket]->SetTransform(&tt);
		}
	}

	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void CharPlayer::PostRender(Scene* pScene)
{
	//glEnable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	m_pShader->UnUse();
	glFrontFace(GL_CW);
	glDisable(GL_BLEND);
}

void CharPlayer::SetWeapon(SocketItem * p)
{
	m_Children[m_iWeaponSocket] = p;
}
