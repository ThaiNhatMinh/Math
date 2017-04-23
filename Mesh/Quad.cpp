
#include "..\iostream.h"


Quad::Quad()
{
	m_vPositions = {
		{1.0f,  1.0f, 0.0f},  // Top Right
		{1.0f, -1.0f, 0.0f},  // Bottom Right
		{-1.0f, -1.0f, 0.0f },  // Bottom Left
		{-1.0f,  1.0f, 0.0f}  // Top Left 
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


Quad::Quad(int axis, float size)
{
	Vec3 vAxis;
	if (axis == 1)
	{
		vAxis.set(1.0, 0.0, 0.0);
		m_vNormals.push_back(vAxis);
		m_vNormals.push_back(vAxis);
		m_vNormals.push_back(vAxis);
		m_vNormals.push_back(vAxis);
		m_vPositions.push_back(Vec3(0.0f, size, size));  // Top Right
		m_vPositions.push_back(Vec3(0.0f, size, -size));  // Bottom Right
		m_vPositions.push_back(Vec3(0.0f, -size, -size));
		m_vPositions.push_back(Vec3(0.0f, -size, size));
	}
	else if (axis == 2)
	{
		vAxis.set(0.0, 1.0, 0.0);
		m_vNormals.push_back(vAxis);
		m_vNormals.push_back(vAxis);
		m_vNormals.push_back(vAxis);
		m_vNormals.push_back(vAxis);
		m_vPositions = {
			{ size, 0.0f ,  size,},  // Top Right
			{ size, 0.0f, -size, },  // Bottom Right
			{ -size, 0.0f, -size, },  // Bottom Left
			{ -size,  0.0f, size, }  // Top Left 
		};
	}
	else
	{
		vAxis.set(0.0, 0.0, 1.0);
		m_vNormals.push_back(vAxis);
		m_vNormals.push_back(vAxis);
		m_vNormals.push_back(vAxis);
		m_vNormals.push_back(vAxis);
		m_vPositions = {
			{ size,  size, 0.0f },  // Top Right
			{ size, -size, 0.0f },  // Bottom Right
			{ -size, -size, 0.0f },  // Bottom Left
			{ -size,  size, 0.0f }  // Top Left 
		};
	}
	
	m_vIndices = {  // Note that we start from 0!
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
