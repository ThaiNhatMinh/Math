#pragma once

#include "..\iostream.h"

mat4 ToMatrix(Vec3 v);

struct FrameBone
{
	Vec3 Pos;
	Vec3 Ort;
	GLuint BoneID;
};

struct Frame
{
	vector<FrameBone> Frames;
	GLuint time;
};
struct Skeleton
{
	mat4 Transform;
	GLuint BoneID;
	GLuint ParentBoneID;
};
class SMDAnimation
{
public:
	string Name;
	GLuint iNumFrame;
	vector<Frame>  m_FrameList;
	GLuint iFPS;
public:
	SMDAnimation(string name) { Name = name; };
	~SMDAnimation() {};

	bool Load(const char* pFile,int);
	void GetFrame(int frame, vector<Bone>& bone,vector<Skeleton>& ske);
};