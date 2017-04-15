#include "..\iostream.h"
#include "..\include\IL\il.h"

SkyBox::SkyBox() :SceneNode("SkyBox")
{
}


SkyBox::~SkyBox()
{
}

void SkyBox::Init()
{
	m_Shader.Load("Game\\Shader\\SkyBox.vs", "Game\\Shader\\SkyBox.frag");
	m_Shader.LinkShader();

	vector<string> filelist;
	filelist.push_back("Game\\Texture\\grimnight\\grimmnight_ft.tga");
	filelist.push_back("Game\\Texture\\grimnight\\grimmnight_bk.tga");
	filelist.push_back("Game\\Texture\\grimnight\\grimmnight_dn.tga");
	filelist.push_back("Game\\Texture\\grimnight\\grimmnight_up.tga");
	filelist.push_back("Game\\Texture\\grimnight\\grimmnight_rt.tga");
	filelist.push_back("Game\\Texture\\grimnight\\grimmnight_lf.tga");
	m_pCubeTex = Resources::LoadCubeTex(filelist);
	//m_Box.Finalize();
	m_fAngle = 0.0f;
}

void SkyBox::OnUpdate(Scene *pScene, float dt)
{
	m_fAngle += dt * 2.5f;
	m_Ort.setRotateYAxis(m_fAngle);

	SceneNode::OnUpdate(pScene, dt);
}

void SkyBox::PreRender(Scene *pScene)
{
	glDisable(GL_CULL_FACE);
	Camera camera = pScene->GetCamera();
	Frustum frustum = pScene->GetFrustum();
	mat4 view = camera.GetViewMatrix();
	view.Translate(0, 0, 0);
	mat4 proj = frustum.GetProjMatrix();
	m_Shader.Use();
	m_Shader.SetUniformMatrix("Model", m_Ort.ToMatrix().ToFloatPtr());
	m_Shader.SetUniformMatrix("View", view.ToFloatPtr());
	m_Shader.SetUniformMatrix("Proj", proj.ToFloatPtr());
	
	glDepthFunc(GL_LEQUAL);

}

void SkyBox::Render(Scene* pScene)
{
	m_Shader.SetUniform("Skybox", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_pCubeTex->iIndex);
	
	glBindVertexArray(m_Box.m_iVAO);
	glDrawArrays(m_Box.Topology, 0, m_Box.m_vPositions.size());
	
	

}

void SkyBox::PostRender(Scene *pScene)
{
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);
	glBindVertexArray(0);
}
/*
void SkyBox::LoadCubeTexture(const vector<string>& filelist)
{
	glGenTextures(1, &m_iCubeTexId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_iCubeTexId);
	int w, h;
	unsigned char* image;
	for (size_t i = 0; i < filelist.size(); i++)
	{
		image = SOIL_load_image(filelist[i].c_str(), &w, &h, 0, SOIL_LOAD_RGB);
		if (!image) Log::Message(Log::LOG_ERROR, "Can't load file: " + filelist[i]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
*/