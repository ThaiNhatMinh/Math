#include "..\iostream.h"



Cube::Cube()
{
	m_vPositions = vector<Vec3>{
		// Positions          
		Vec3 (-1.0f,  1.0f, -1.0f),
		Vec3(-1.0f, -1.0f, -1.0f),
		Vec3 (1.0f, -1.0f, -1.0f),
		Vec3(1.0f, -1.0f, -1.0f),
		Vec3(1.0f,  1.0f, -1.0f),
		Vec3(-1.0f,  1.0f, -1.0f),

		Vec3(-1.0f, -1.0f,  1.0f),
		Vec3(-1.0f, -1.0f, -1.0f),
		Vec3(-1.0f,  1.0f, -1.0f),
		Vec3(-1.0f,  1.0f, -1.0f),
		Vec3(-1.0f,  1.0f,  1.0f),
		Vec3(-1.0f, -1.0f,  1.0f),

		Vec3(1.0f, -1.0f, -1.0f),
		Vec3(1.0f, -1.0f,  1.0f),
		Vec3(1.0f,  1.0f,  1.0f),
		Vec3(1.0f,  1.0f,  1.0f),
		Vec3(1.0f,  1.0f, -1.0f),
		Vec3(1.0f, -1.0f, -1.0f),

		Vec3(-1.0f, -1.0f,  1.0f),
		Vec3(-1.0f,  1.0f,  1.0f),
		Vec3(1.0f,  1.0f,  1.0f),
		Vec3(1.0f,  1.0f,  1.0f),
		Vec3(1.0f, -1.0f,  1.0f),
		Vec3(-1.0f, -1.0f,  1.0f),

		Vec3(-1.0f,  1.0f, -1.0f),
		Vec3(1.0f,  1.0f, -1.0f),
		Vec3(1.0f,  1.0f,  1.0f),
		Vec3(1.0f,  1.0f,  1.0f),
		Vec3(-1.0f,  1.0f,  1.0f),
		Vec3(-1.0f,  1.0f, -1.0f),

		Vec3(-1.0f, -1.0f, -1.0f),
		Vec3(-1.0f, -1.0f,  1.0f),
		Vec3(1.0f, -1.0f, -1.0f),
		Vec3(1.0f, -1.0f, -1.0f),
		Vec3(-1.0f, -1.0f,  1.0f),
		Vec3(1.0f, -1.0f,  1.0f)
	};
	m_vUVs = std::vector<Vec2>{
		Vec2(0.0f, 0.0f),
		Vec2(1.0f, 0.0f),
		Vec2(1.0f, 1.0f),
		Vec2(1.0f, 1.0f),
		Vec2(0.0f, 1.0f),
		Vec2(0.0f, 0.0f),

		Vec2(0.0f, 0.0f),
		Vec2(1.0f, 0.0f),
		Vec2(1.0f, 1.0f),
		Vec2(1.0f, 1.0f),
		Vec2(0.0f, 1.0f),
		Vec2(0.0f, 0.0f),

		Vec2(1.0f, 0.0f),
		Vec2(1.0f, 1.0f),
		Vec2(0.0f, 1.0f),
		Vec2(0.0f, 1.0f),
		Vec2(0.0f, 0.0f),
		Vec2(1.0f, 0.0f),

		Vec2(1.0f, 0.0f),
		Vec2(1.0f, 1.0f),
		Vec2(0.0f, 1.0f),
		Vec2(0.0f, 1.0f),
		Vec2(0.0f, 0.0f),
		Vec2(1.0f, 0.0f),

		Vec2(0.0f, 1.0f),
		Vec2(1.0f, 1.0f),
		Vec2(1.0f, 0.0f),
		Vec2(1.0f, 0.0f),
		Vec2(0.0f, 0.0f),
		Vec2(0.0f, 1.0f),

		Vec2(0.0f, 1.0f),
		Vec2(1.0f, 1.0f),
		Vec2(1.0f, 0.0f),
		Vec2(1.0f, 0.0f),
		Vec2(0.0f, 0.0f),
		Vec2(0.0f, 1.0f),
	};
	m_vNormals = std::vector<Vec3>{
		Vec3(0.0f,  0.0f, -1.0f),
		Vec3(0.0f,  0.0f, -1.0f),
		Vec3(0.0f,  0.0f, -1.0f),
		Vec3(0.0f,  0.0f, -1.0f),
		Vec3(0.0f,  0.0f, -1.0f),
		Vec3(0.0f,  0.0f, -1.0f),

		Vec3(0.0f,  0.0f,  1.0f),
		Vec3(0.0f,  0.0f,  1.0f),
		Vec3(0.0f,  0.0f,  1.0f),
		Vec3(0.0f,  0.0f,  1.0f),
		Vec3(0.0f,  0.0f,  1.0f),
		Vec3(0.0f,  0.0f,  1.0f),

		Vec3(-1.0f,  0.0f,  0.0f),
		Vec3(-1.0f,  0.0f,  0.0f),
		Vec3(-1.0f,  0.0f,  0.0f),
		Vec3(-1.0f,  0.0f,  0.0f),
		Vec3(-1.0f,  0.0f,  0.0f),
		Vec3(-1.0f,  0.0f,  0.0f),

		Vec3(1.0f,  0.0f,  0.0f),
		Vec3(1.0f,  0.0f,  0.0f),
		Vec3(1.0f,  0.0f,  0.0f),
		Vec3(1.0f,  0.0f,  0.0f),
		Vec3(1.0f,  0.0f,  0.0f),
		Vec3(1.0f,  0.0f,  0.0f),

		Vec3(0.0f, -1.0f,  0.0f),
		Vec3(0.0f, -1.0f,  0.0f),
		Vec3(0.0f, -1.0f,  0.0f),
		Vec3(0.0f, -1.0f,  0.0f),
		Vec3(0.0f, -1.0f,  0.0f),
		Vec3(0.0f, -1.0f,  0.0f),

		Vec3(0.0f,  1.0f,  0.0f),
		Vec3(0.0f,  1.0f,  0.0f),
		Vec3(0.0f,  1.0f,  0.0f),
		Vec3(0.0f,  1.0f,  0.0f),
		Vec3(0.0f,  1.0f,  0.0f),
		Vec3(0.0f,  1.0f,  0.0f),
	};

	Topology = GL_TRIANGLES;
	Finalize();
}

Cube::Cube(float size)
{
	m_vPositions = vector<Vec3>{
		Vec3(-size, -size, -size),
		Vec3(size, -size, -size),
		Vec3(size,  size, -size),
		Vec3(size,  size, -size),
		Vec3(-size,  size, -size),
		Vec3(-size, -size, -size),

		Vec3(-size, -size,  size),
		Vec3(size, -size,  size),
		Vec3(size,  size,  size),
		Vec3(size,  size,  size),
		Vec3(-size,  size,  size),
		Vec3(-size, -size,  size),

		Vec3(-size,  size,  size),
		Vec3(-size,  size, -size),
		Vec3(-size, -size, -size),
		Vec3(-size, -size, -size),
		Vec3(-size, -size,  size),
		Vec3(-size,  size,  size),

		Vec3(size,  size,  size),
		Vec3(size,  size, -size),
		Vec3(size, -size, -size),
		Vec3(size, -size, -size),
		Vec3(size, -size,  size),
		Vec3(size,  size,  size),

		Vec3(-size, -size, -size),
		Vec3(size, -size, -size),
		Vec3(size, -size,  size),
		Vec3(size, -size,  size),
		Vec3(-size, -size,  size),
		Vec3(-size, -size, -size),

		Vec3(-size,  size, -size),
		Vec3(size,  size, -size),
		Vec3(size,  size,  size),
		Vec3(size,  size,  size),
		Vec3(-size,  size,  size),
		Vec3(-size,  size, -size),
	};
	m_vUVs = std::vector<Vec2>{
		Vec2(0.0f, 0.0f),
		Vec2(1.0f, 0.0f),
		Vec2(1.0f, 1.0f),
		Vec2(1.0f, 1.0f),
		Vec2(0.0f, 1.0f),
		Vec2(0.0f, 0.0f),

		Vec2(0.0f, 0.0f),
		Vec2(1.0f, 0.0f),
		Vec2(1.0f, 1.0f),
		Vec2(1.0f, 1.0f),
		Vec2(0.0f, 1.0f),
		Vec2(0.0f, 0.0f),

		Vec2(1.0f, 0.0f),
		Vec2(1.0f, 1.0f),
		Vec2(0.0f, 1.0f),
		Vec2(0.0f, 1.0f),
		Vec2(0.0f, 0.0f),
		Vec2(1.0f, 0.0f),

		Vec2(1.0f, 0.0f),
		Vec2(1.0f, 1.0f),
		Vec2(0.0f, 1.0f),
		Vec2(0.0f, 1.0f),
		Vec2(0.0f, 0.0f),
		Vec2(1.0f, 0.0f),

		Vec2(0.0f, 1.0f),
		Vec2(1.0f, 1.0f),
		Vec2(1.0f, 0.0f),
		Vec2(1.0f, 0.0f),
		Vec2(0.0f, 0.0f),
		Vec2(0.0f, 1.0f),

		Vec2(0.0f, 1.0f),
		Vec2(1.0f, 1.0f),
		Vec2(1.0f, 0.0f),
		Vec2(1.0f, 0.0f),
		Vec2(0.0f, 0.0f),
		Vec2(0.0f, 1.0f),
	};
	m_vNormals = std::vector<Vec3>{
		Vec3(0.0f,  0.0f, -1.0f),
		Vec3(0.0f,  0.0f, -1.0f),
		Vec3(0.0f,  0.0f, -1.0f),
		Vec3(0.0f,  0.0f, -1.0f),
		Vec3(0.0f,  0.0f, -1.0f),
		Vec3(0.0f,  0.0f, -1.0f),

		Vec3(0.0f,  0.0f,  1.0f),
		Vec3(0.0f,  0.0f,  1.0f),
		Vec3(0.0f,  0.0f,  1.0f),
		Vec3(0.0f,  0.0f,  1.0f),
		Vec3(0.0f,  0.0f,  1.0f),
		Vec3(0.0f,  0.0f,  1.0f),

		Vec3(-1.0f,  0.0f,  0.0f),
		Vec3(-1.0f,  0.0f,  0.0f),
		Vec3(-1.0f,  0.0f,  0.0f),
		Vec3(-1.0f,  0.0f,  0.0f),
		Vec3(-1.0f,  0.0f,  0.0f),
		Vec3(-1.0f,  0.0f,  0.0f),

		Vec3(1.0f,  0.0f,  0.0f),
		Vec3(1.0f,  0.0f,  0.0f),
		Vec3(1.0f,  0.0f,  0.0f),
		Vec3(1.0f,  0.0f,  0.0f),
		Vec3(1.0f,  0.0f,  0.0f),
		Vec3(1.0f,  0.0f,  0.0f),

		Vec3(0.0f, -1.0f,  0.0f),
		Vec3(0.0f, -1.0f,  0.0f),
		Vec3(0.0f, -1.0f,  0.0f),
		Vec3(0.0f, -1.0f,  0.0f),
		Vec3(0.0f, -1.0f,  0.0f),
		Vec3(0.0f, -1.0f,  0.0f),

		Vec3(0.0f,  1.0f,  0.0f),
		Vec3(0.0f,  1.0f,  0.0f),
		Vec3(0.0f,  1.0f,  0.0f),
		Vec3(0.0f,  1.0f,  0.0f),
		Vec3(0.0f,  1.0f,  0.0f),
		Vec3(0.0f,  1.0f,  0.0f),
	};

	Topology = GL_TRIANGLES;
	Finalize();
}

Cube::~Cube()
{
}
