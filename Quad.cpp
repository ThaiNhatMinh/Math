
#include "Quad.h"


Quad::Quad()
{
	m_vPositions = {
		{0.5f,  0.5f, 0.0f},  // Top Right
		{0.5f, -0.5f, 0.0f},  // Bottom Right
		{-0.5f, -0.5f, 0.0f },  // Bottom Left
		{-0.5f,  0.5f, 0.0f}  // Top Left 
	};
	m_vIndices ={  // Note that we start from 0!
		0, 1, 3,  // First Triangle
		1, 2, 3   // Second Triangle
	};

	m_vUVs = {
		Vec2(1,1),
		Vec2(1,0),
		Vec2(0,0),
		Vec2(0,1) };
	Topology = GL_TRIANGLES;



	Finalize();
}


Quad::~Quad()
{
}
