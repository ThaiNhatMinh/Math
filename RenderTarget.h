#pragma once
#include "iostream.h"

class RenderTarget
{
private:
	GLuint m_iTexture;	// index 
	GLuint m_iWidth;	//screen height
	GLuint m_iHeight;	// screen width
	GLuint m_iFBO;		// frame buffer object
	GLuint m_iRBO;		// render buffer object
public:
	RenderTarget(int w, int h);
	~RenderTarget();

	void BeginFrame();
	void EndFrame();
	void Resize(int w, int h);
	GLuint TextureID() { return m_iTexture; };
};

