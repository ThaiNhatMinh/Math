#pragma once
#include "..\iostream.h"
struct Texture
{
	char szName[64];
	GLuint iIndex;
	GLuint iWidth;
	GLuint iHeight;
	inline void Bind(GLuint tex_unit) {
		glActiveTexture(GL_TEXTURE0 + tex_unit);
		glBindTexture(GL_TEXTURE_2D, iIndex);
	};
	inline void UnBind() { glBindTexture(GL_TEXTURE_2D, 0); };
};