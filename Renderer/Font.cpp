#include "..\iostream.h"



void Font::InitBuffer()
{
	glGenVertexArrays(1, &m_iVAO);
	glGenBuffers(1, &m_iVBO);

	glBindVertexArray(m_iVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_iVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6*4, NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);
}

Font::Font()
{
}


Font::~Font()
{
}

bool Font::Init(float w,float h)
{
	m_Proj = Math::Ortho(w, h);
	InitBuffer();
	m_Shader.Load("Game\\Shader\\Text2D.vs", "Game\\Shader\\Text2D.frag");
	m_Shader.LinkShader();
	FT_Library lib;
	if (FT_Init_FreeType(&lib))
	{
		Log::Message(Log::LOG_ERROR, "Can't Init FreeType libary.");
		return false;
	}

	FT_Face face;
	if (FT_New_Face(lib, "Game\\Fonts\\arial.ttf", 0, &face))
	{
		Log::Message(Log::LOG_ERROR, "Can't Load Font file.");
		return false;
	}

	FT_Set_Pixel_Sizes(face, 0, 14);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (GLubyte i = 0; i < 128; i++)
	{

		if (FT_Load_Char(face, i, FT_LOAD_RENDER))
		{
			Log::Message(Log::LOG_WARNING, "Can't load char.");
			continue;
		}

		GLuint texID;
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character;
		character.iTextureID = texID;
		character.size[0] = face->glyph->bitmap.width;
		character.size[1] = face->glyph->bitmap.rows;
		character.Bearing[0] = face->glyph->bitmap_left;
		character.Bearing[1] = face->glyph->bitmap_top;
		character.advance = face->glyph->advance.x;
	
		m_Characters.insert(std::pair<GLchar, Character>(i, character));
		//cout << face->glyph->bitmap_left << " " << face->glyph->bitmap_top << endl;
	}

	FT_Done_Face(face);
	FT_Done_FreeType(lib);
	return true;
}

void Font::Draw( string text, float x, float y, float scale, Vec3 color)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_Shader.Use();
	m_Shader.SetUniform("InColor", color.x, color.y, color.z);
	m_Shader.SetUniformMatrix("Proj", m_Proj.ToFloatPtr());

	glBindVertexArray(m_iVAO);
	
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = m_Characters[*c];
		float xpos = x + ch.Bearing[0] * scale;
		float ypos = y - (ch.size[1] - ch.Bearing[1])*scale;
		GLfloat w = ch.size[0] * scale;
		GLfloat h = ch.size[1] * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};

		// Render glyph texture over quad

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ch.iTextureID);
		m_Shader.SetUniform("TexFont", 0);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, m_iVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		//cout << xpos <<' ' << ypos<< endl;
		x += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_BLEND);
}
