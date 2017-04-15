#pragma once
#include "..\iostream.h"

struct WeightSet
{
	int iBone;
	float fWeight;
	WeightSet(int b, float w):iBone(b), fWeight(w) {};
	WeightSet() {};
};

class SMDMesh : public Mesh
{
public:
	string m_Material; // now it just a string
	vector<GLuint> m_ParentBone;
	vector<WeightSet> m_Weights;
public:
	SMDMesh();
	~SMDMesh();
	void Finalize();
};