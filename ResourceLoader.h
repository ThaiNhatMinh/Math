#pragma once
#include "iostream.h"

struct cl_texture_t
{
	char szName[64];
	GLuint iIndex;
	GLuint iWidth;
	GLuint iHeight;
	inline void Bind(GLuint tex_unit) {
		glActiveTexture(GL_TEXTURE0 + tex_unit);
		glBindTexture(GL_TEXTURE_2D, iIndex); };
	inline void UnBind() { glBindTexture(GL_TEXTURE_2D, 0); };
};


class Resources
{
private:
	static vector<cl_texture_t*> m_Textures;

private:
	static cl_texture_t* HasTexture(const char* filename);

public:
	Resources();
	~Resources();
	static cl_texture_t* LoadTexture(const char* filename);
	static void Release();

};

