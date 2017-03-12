#pragma once

#include "..\iostream.h"


class FrameBuffer {

protected:
	GLuint m_iTexture;	// index 
	GLuint m_iWidth;	//screen height
	GLuint m_iHeight;	// screen width
	GLuint m_iFBO;		// frame buffer object
	GLuint m_iRBO;		// render buffer object
public:
	FrameBuffer(int w, int h) :m_iWidth(w),m_iHeight(h) {};
	~FrameBuffer() {};

	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;
	virtual void Resize(int w, int h) = 0;
	GLuint TextureID() { return m_iTexture; };
};