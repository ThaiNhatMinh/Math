#ifndef __LIGHT__
#define __LIGHT__

#include "..\Common\Shader.h"

// simple interface of light
struct ILight
{
	virtual void SendData(Shader* pShader) = 0;

	// light can be update for some data
	virtual void Update(float dt) = 0;
	
	// this function using for debug
	//virtual void Render();
};

#endif