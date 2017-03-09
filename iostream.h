#ifndef __HEADER__
#define __HEADER__

#include <iostream>
#include <string>
#include <vector>

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


#include "Common.h"
#include "Timer.h"
#include "Log.h"
#include "Shader.h"
//#include "Cube.h"
#include "ResourceLoader.h"

#include "ILight.h"
#include "DirectionLight.h"
#include "PointLight.h"
#include "Spotlight.h"

#include "RenderTarget.h"
#endif