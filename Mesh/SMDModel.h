#pragma once
#include "..\iostream.h"
struct Bone
{
	char Name[64];		// Name of bone
	GLint iID;			// Bone id
	GLint iParentID;	// Parent bone id
	Vec3 vPos;			//
	Vec3 vOrt;			//
	mat4 Pose;
};

class SMDFile
{
public:
	GLuint m_iVersion;
	vector<Bone> m_BoneList;
	vector<SMDMesh*> m_MeshList;
public:
	SMDFile();
	~SMDFile();

	bool Init(const char* pFilename);
	void SetupMesh();
	SMDMesh* GetMesh(int id) { return m_MeshList[id]; };
	vector<SMDMesh*>& GetMesh() { return m_MeshList; };
	vector<Bone>& GetBone() { return m_BoneList; };
};
