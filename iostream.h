#ifndef __HEADER__
#define __HEADER__

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
// this file include basic I/O and basic data struct: string, vector
using std::cout;
using std::endl;
using std::string;
using std::vector;


// OpenGL Include
// GLEW
#define GLEW_STATIC
#include "GL/glew.h"
// GLFW
#include "GLFW/glfw3.h"

// Math
#include "Math\Vector.h"
#include "Math\CMath.h"
#include "Math\Matrix.h"
#include "Math\Quaternion.h"

//Common
#include "Common\Common.h"
#include "Common\Timer.h"
#include "Common\Log.h"
#include "Common\Shader.h"
#include "Camera.h"
// Mesh 
#include "Mesh\Mesh.h"
#include "Mesh\Quad.h"
#include "Mesh\Cube.h"
#include "Mesh\Axis.h"

// Resouce 
#include "Resource\ResourceLoader.h"


// Light
#include "Light\ILight.h"
#include "Light\DirectionLight.h"
#include "Light\PointLight.h"
#include "Light\Spotlight.h"

// renderer
#include "Renderer\SkyBox.h"
#include "Renderer\Font.h"
#include "Renderer\FrameBuffer.h"
#include "Renderer\RenderTarget.h"
#include "Renderer\MSSA.h"

// Windows
#include "Windows.h"
#endif