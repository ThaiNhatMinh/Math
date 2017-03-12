#pragma once
#include "..\iostream.h"

class RenderTarget: public FrameBuffer
{

public:
	RenderTarget(int w, int h);
	~RenderTarget();

	void BeginFrame();
	void EndFrame();
	void Resize(int w, int h);
	GLuint FBO() { return m_iFBO; };
};

