#pragma once
#include "Mesh.h"
class Quad : public Mesh
{
public:
	Quad();
	Quad(int axis,float size);
	~Quad();
};

