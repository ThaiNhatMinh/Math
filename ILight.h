#ifndef __LIGHT__
#define __LIGHT__

#include "Shader.h"

// simple interface of light
struct ILight
{
	virtual void SendData(Shader* pShader) = 0;
	virtual void Update(float dt) = 0;
};

#endif