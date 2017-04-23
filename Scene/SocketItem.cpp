#include "..\iostream.h"

//vector<SocketItem*> SocketItem::m_pList;
void SocketItem::RenderMesh(LTBMesh * mesh)
{
	mesh->m_pTexture->Bind(0);
	glBindVertexArray(mesh->m_iVAO);
	//glDrawArrays(mesh->Topology, 0, mesh->m_vPositions.size());
	glDrawElements(mesh->Topology, mesh->m_vIndices.size(), GL_UNSIGNED_INT, 0);
	mesh->m_pTexture->UnBind();
}

void SocketItem::LoadModel(const char * FileName)
{
	m_pAnimation = new BaseAnim;
	vector<Animation*>& v = m_pAnimation->Get();
	if (LTBFile::BeginLoad(FileName))
	{
		m_pProp			= LTBFile::LoadProp();
		m_pMeshs		= LTBFile::LoadMesh();
		m_pNodeLists	= LTBFile::LoadSkeleton();
						  LTBFile::LoadWS();
						  LTBFile::LoadChildName();
		v				= LTBFile::LoadAnimation(m_pNodeLists);
						  LTBFile::LoadSocket();
						  LTBFile::EndLoad();
	}
	else
	{
		Log::Message(Log::LOG_ERROR, "Can't open file: " + string(FileName));
		return;
	}

	for (size_t i = 0; i < m_pProp->m_nPieces; i++)
	{
		m_pMeshs[i]->Topology = GL_TRIANGLES;
		m_pMeshs[i]->Finalize(m_pShader);
	}
}

void SocketItem::LoadTexture(const char * FileName)
{
	string file = FileName;
	size_t t1 = file.find_last_of("\\/");
	size_t t2 = file.size() - t1;
	string texFile = file.substr(t1, t2 - 4) + ".DTX";
	string TexPathFile = (file.substr(0, t1)) + texFile;
	for (size_t i = 0; i < m_pProp->m_nPieces; i++)
	{
		m_pMeshs[i]->m_pTexture = Resources::LoadDTX(TexPathFile.c_str());
	}
}

SocketItem::SocketItem():SceneNode("SocketItem")
{
	//m_pList.push_back(this);
}

SocketItem::~SocketItem()
{
	//delete m_pShader;
	
	delete m_pAnimation;
	for (GLuint i = 0; i < m_pNodeLists.size(); i++)
		delete m_pNodeLists[i];
	for (size_t i = 0; i < m_pMeshs.size(); i++)
		delete m_pMeshs[i];
	delete m_pProp;
}

void SocketItem::Init(const char * FileName)
{
	strcpy(m_FileName, FileName);

	m_pShader = ShaderManager::GetLoad("Socket","Game\\Shader\\Simple.vs", "Game\\Shader\\Skeleton.frag");
	m_pShader->LinkShader();
	m_iModelUniform = m_pShader->GetUniformLocation("Model");
	m_iViewUniform = m_pShader->GetUniformLocation("View");
	m_iProjUniform = m_pShader->GetUniformLocation("Proj");
	m_iLightPosUniform = m_pShader->GetUniformLocation("lightPos");
	m_iEyePosUniform = m_pShader->GetUniformLocation("EyePos");



	LoadModel(FileName);
	LoadTexture(FileName);

}

void SocketItem::OnUpdate(Scene * pScene, float dt)
{
}

void SocketItem::PreRender(Scene * pScene)
{
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
	//glDisable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
}

void SocketItem::Render(Scene *)
{
	for (size_t i = 0; i < m_pProp->m_nPieces; i++)
		RenderMesh(m_pMeshs[i]);
}

void SocketItem::PostRender(Scene *)
{
	glFrontFace(GL_CW);
	//glEnable(GL_CULL_FACE);
}
