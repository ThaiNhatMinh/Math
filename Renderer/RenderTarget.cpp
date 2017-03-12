#include "..\iostream.h"



RenderTarget::RenderTarget(int w, int h):FrameBuffer(w,h)
{
	/* Create back-buffer, used for post-processing */

	
	
	
	/* Texture */
	glGenTextures(1, &m_iTexture);
	glBindTexture(GL_TEXTURE_2D, m_iTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	/* Depth buffer */
	glGenRenderbuffers(1, &m_iRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_iRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	/* Framebuffer to link everything together */
	glGenFramebuffers(1, &m_iFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_iFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_iTexture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_iRBO);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		Log::Message(Log::LOG_ERROR, "Framebuffer not complete.");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);




}


RenderTarget::~RenderTarget()
{
	glDeleteRenderbuffers(1, &m_iRBO);
	glDeleteTextures(1, &m_iTexture);
	glDeleteFramebuffers(1, &m_iFBO);
}

void RenderTarget::BeginFrame()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_iFBO);
}

void RenderTarget::EndFrame()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTarget::Resize(int w, int h)
{
	glBindTexture(GL_TEXTURE_2D, m_iTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, m_iRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, w, h);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	m_iWidth = w;
	m_iHeight = h;
}
