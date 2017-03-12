#pragma once
#include "..\iostream.h"

// multisample anti-aliasing

class MSSA : public FrameBuffer
{
private:
	GLuint m_iNumSample;
	GLuint m_iDrawFBO;
public:
	MSSA(int w, int h, int numsample);
	~MSSA() {};

	void BeginFrame();
	void EndFrame();
	void Resize(int w, int h);
	void SetDrawFBO(GLuint fbo) {		m_iDrawFBO = fbo;	};
};