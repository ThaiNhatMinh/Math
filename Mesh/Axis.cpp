#include "..\iostream.h"



Axis::Axis(float size)
{
	m_vPositions = {
		Vec3(size*2,0,0),
		Vec3(0,size*1.5f,0),
		Vec3(0,0,size),
		Vec3(0,0,0)
	};

	m_vIndices = { 0,3,1,3,2,3 };

	Topology = GL_LINES;

	Finalize();
}


Axis::~Axis()
{
}
