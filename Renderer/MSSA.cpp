#include "..\iostream.h"

MSSA::MSSA(int w,int h,int numsample):FrameBuffer(w,h)
{
	glGenTextures(1,&m_iTexture);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_iTexture);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, numsample, GL_RGB, w, h, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);


	glGenFramebuffers(1, &m_iFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_iFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_iTexture, 0);

	glGenRenderbuffers(1, &m_iRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_iRBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, numsample, GL_DEPTH24_STENCIL8, w, h);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_iRBO);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		Log::Message(Log::LOG_ERROR, "Framebuffer not complete.");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_iNumSample = numsample;
	m_iDrawFBO = 0;
}

void MSSA::BeginFrame()
{

	
	glBindFramebuffer(GL_FRAMEBUFFER, m_iFBO);
}

void MSSA::EndFrame()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_iFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_iDrawFBO);
	glBlitFramebuffer(0, 0, m_iWidth, m_iHeight, 0, 0, m_iWidth, m_iHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	
}

void MSSA::Resize(int w, int h)
{
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_iTexture);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_iNumSample, GL_RGB, w, h, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, m_iRBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_iNumSample, GL_DEPTH24_STENCIL8, w, h);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	m_iWidth = w;
	m_iHeight = h;
}
