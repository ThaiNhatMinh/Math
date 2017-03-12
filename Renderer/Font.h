#pragma once

#include "..\iostream.h"
#include <ft2build.h>
#include <map>
#include FT_FREETYPE_H
#pragma comment(lib,"freetype271d.lib")

struct Character {
	GLuint iTextureID;
	GLuint size[2];
	GLuint Bearing[2];
	GLuint advance;
};

class Font
{
private:
	std::map<GLchar, Character> m_Characters;
	GLuint m_iVAO;
	GLuint m_iVBO;
	mat4 m_Proj;
	Shader m_Shader;
private:
	void InitBuffer();
public:
	Font();
	~Font();
	bool Init(int w,int h);
	void Draw( string text, float x, float y, float scale = 1.0f, Vec3 color = Vec3(1.0f));
};

