#include "..\iostream.h"



SkyBox::SkyBox()
{
}


SkyBox::~SkyBox()
{
}

void SkyBox::Init()
{
	m_Shader.Load("Game\\Shader\\SkyBox.vs", "Game\\Shader\\SkyBox.frag");
	//m_Box.Finalize();
}

void SkyBox::Render(mat4& view,mat4& proj)
{
	glDepthMask(GL_FALSE);
	m_Shader.Use();
	m_Shader.SetUniformMatrix("View", view.ToFloatPtr());
	m_Shader.SetUniformMatrix("Proj", proj.ToFloatPtr());
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_iCubeTexId);
	m_Shader.SetUniform("Skybox", 0);

	glBindVertexArray(m_Box.m_iVAO);
	glDrawArrays(m_Box.Topology, 0, m_Box.m_vPositions.size());
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);

}

void SkyBox::LoadCubeTexture(const vector<string>& filelist)
{
	glGenTextures(1, &m_iCubeTexId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_iCubeTexId);
	int w, h;
	unsigned char* image;
	for (int i = 0; i < filelist.size(); i++)
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
