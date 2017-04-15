#pragma once

#include "..\iostream.h"


struct Weight
{
	Weight(int b, float w) { Bone = (float)b; weight = w; };
	Weight() {};
	float Bone;
	float weight;
};
typedef vector<Weight> WeightList;


struct Vertex
{
	Vec3		Pos;
	Vec3		Normal;
	Vec2		UV;
	Weight		Weights[4];
	Vertex()
	{
		Weights[0].weight = 0;
		Weights[1].weight = 0;
		Weights[2].weight = 0;
		Weights[3].weight = 0;
	}
};
typedef std::vector<Vertex> VertexList;

struct Triangle
{
	int             m_Indices[3];
	Triangle(int a, int b, int c) { m_Indices[0] = a; m_Indices[1] = b; m_Indices[2] = c; }

};
typedef std::vector<Triangle> TriangleList;





class LTBMesh : public Mesh
{
public:
	string m_Name;
	vector<Vertex> m_vVertex;
public:
	LTBMesh();
	~LTBMesh();
	void Finalize(Shader*);
};