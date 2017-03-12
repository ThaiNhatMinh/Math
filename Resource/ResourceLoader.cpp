#include "ResourceLoader.h"
#include "..\SOIL.h"

#pragma comment(lib,"SOIL.lib")
vector<Texture*> Resources::m_Textures;

Texture * Resources::HasTexture(const char * filename)
{
	for (size_t i = 0; i < m_Textures.size(); i++)
		if (!strcmp(m_Textures[i]->szName, filename))
			return m_Textures[i];

	return NULL;
}

Resources::Resources()
{
}


Resources::~Resources()
{
	
}

Texture * Resources::LoadTexture(const char * filename)
{
	Texture* tex=NULL;
	if ((tex = HasTexture(filename)) != NULL) return tex;

	GLint width, height;

	unsigned char* image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);
	if (!image)
	{
		Log::Message(Log::LOG_ERROR, "Can't load texture " + string(filename));
		return NULL;
	}
	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glBindTexture(GL_TEXTURE_2D, 0);
	//glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);

	tex = new Texture;
	sprintf(tex->szName, "%s", filename);
	tex->iIndex = id;
	tex->iWidth = width;
	tex->iHeight = height;

	m_Textures.push_back(tex);


	return tex;
}

Texture * Resources::LoadDTX(const char * filename)
{
	Texture* tex = NULL;
	if ((tex = HasTexture(filename)) != NULL) return tex;

	GLint width, height;

	return nullptr;
}

void Resources::Release()
{
	for (size_t i = 0; i < m_Textures.size(); i++)
		delete m_Textures[i];
}
