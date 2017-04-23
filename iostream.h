#ifndef __HEADER_3D__
#define __HEADER_3D__


#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <map>
// this file include basic I/O and basic data struct: string, vector
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::map;

// OpenGL Include
// GLEW
#define GLEW_STATIC
#include "GL/glew.h"
// GLFW
#include "GLFW/glfw3.h"

// Math
#include "Math\Vector.h"
#include "Math\Matrix.h"
#include "Math\Quaternion.h"
#include "Math\Plane.h"
#include "Math\Frustum.h"
#include "Math\AABB.h"
#include "Math\Sphere.h"

#include "Math\CMath.h"
//Common
#include "Common\Common.h"
#include "Common\Timer.h"
#include "Common\Log.h"
#include "Common\Shader.h"
#include "Resource\ShaderManager.h"
#include "Common\Input.h"
#include "Camera.h"
#include "Scene\TPSCamera.h"


// Light
#include "Light\ILight.h"
#include "Light\DirectionLight.h"
#include "Light\PointLight.h"
#include "Light\Spotlight.h"


// Resouce 
#include "Resource\Texture.h"
#include "Resource\ResourceLoader.h"

#include "Scene\Interface.h"
#include "Scene\SceneNode.h"
#include "Scene\Scene.h"
// Mesh 
#include "Mesh\Mesh.h"
#include "Mesh\Quad.h"
#include "Mesh\Cube.h"
#include "Mesh\Axis.h"

#include "Mesh\SMDMesh.h"
#include "Mesh\SMDModel.h"
#include "Mesh\SMDAnimation.h"
#include "Mesh\SMDRenderer.h"

#include "Mesh\LTBMesh.h"
#include "Mesh\LTBFile.h"
#include "Mesh\SkeletonNode.h"
#include "Scene\PlayerMotion.h"
#include "Scene\SocketItem.h"
#include "Scene\ModelRender.h"
#include "Mesh\CharPlayer.h"
#include "Mesh\Monster.h"

#include "World\BSPMesh.h"
#include "World\Quake3BSP.h"
#include "World\MapRenderer.h"



// renderer
#include "Renderer\SkyBox.h"
#include "Renderer\Font.h"
#include "Renderer\FrameBuffer.h"
#include "Renderer\RenderTarget.h"
#include "Renderer\MSSA.h"


// World
//#include "World\HeightMap.h"
#include "World\Terrain.h"
// Windows
#include "Windows.h"

extern Font font;
extern Input gInput;
extern GameTimer gTimer;
#endif