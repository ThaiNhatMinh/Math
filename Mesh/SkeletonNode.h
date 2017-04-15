#pragma once

#include "..\iostream.h"

struct AnimKeyFrame
{
	unsigned int	m_Time;
	// A string of information about this key..
	std::string			m_pString;
};
struct FrameData
{
	Vec3 m_Pos;
	Quat m_Ort;
};


// Animation of a joint;
struct NodeAnim
{
	std::vector<AnimKeyFrame> m_KeyFrames;
	std::string m_NameAnimation;
	std::vector<FrameData> m_Frames;
};


struct LTAJoint
{
	int parent;
	mat4 LocalTransform;
};
typedef std::vector<LTAJoint> JointList;
// All animation in here
typedef std::vector<NodeAnim> AnimationList;


class SkeletonNode
{
public:
	SkeletonNode* m_pParent;
	
	string m_Name;
	unsigned int m_Index;
	mat4 m_TransformLocal;
	mat4 m_GlobalTransform;
	mat4 m_InvBindPose;
	vector<SkeletonNode*> m_pChildren;
	AnimationList       m_Anims;
	//Vec3	m_FinalPos;
protected:
	FrameData InterpolateFrame(unsigned int& currentFrame, const NodeAnim& Anim);
public:
	SkeletonNode();
	~SkeletonNode();
	void Update(unsigned int& ,int , JointList& skeleton);
	bool LoadAnimation(FILE* pFile, char* AnimName, const std::vector<AnimKeyFrame>& m_KeyFrames, unsigned int compresstype);
	bool LoadSkeleton(FILE* pFile);
};


struct SkeNode
{
	string m_Name;
	GLint m_Index,m_ParentIndex;
	mat4 m_GlobalTransform;
	mat4 m_InvBindPose;
};

// animation data in one node
typedef vector<FrameData> AnimData;
struct AnimNode
{
	GLint Parent;
	AnimData Data;
};
struct Animation
{
	string Name;
	std::vector<AnimKeyFrame> KeyFrames;
	std::vector<AnimNode> AnimNodeLists;
};

class SkeAnim
{
public:
	vector<Animation*> m_pAnimList;
	unsigned int m_CompressionType;
	unsigned int m_InterpolationMS;
	float m_fTime;
	float m_fSpeed;
	GLuint m_iCurrentFrame;
	GLuint m_iCurrentAnim;

protected:
	void ReadData(FILE* pFile, AnimNode& node,const vector<AnimKeyFrame>&);
	FrameData InterpolateFrame(unsigned int& currentFrame, const AnimNode& Anim,const vector<AnimKeyFrame>&);
public:
	SkeAnim();
	~SkeAnim();
	void SetAnim(int i) { m_iCurrentAnim = i; };
	void NextAnim() { m_iCurrentAnim++; if (m_iCurrentAnim == m_pAnimList.size()) m_iCurrentAnim = 0; };
	bool LoadAnimation(FILE* pFile,const vector<SkeNode*>& pNodeList);
	void Update(float dt, JointList& skeleton);
};