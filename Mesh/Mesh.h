#pragma once
#include "..\iostream.h"


class Mesh
{
public:
	unsigned int m_iVAO = 0;
	unsigned int m_iVBO;
	unsigned int m_iEBO;
	vector<Vec3> m_vPositions;
	vector<Vec3> m_vNormals;
	vector<Vec2> m_vUVs;
	vector<unsigned int> m_vIndices;
	GLuint Topology;
	Texture* m_pTexture; // If there is no texture. It will use color
public:
	Mesh();
	~Mesh();
	void Finalize();
	
};

