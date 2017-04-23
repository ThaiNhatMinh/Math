#include "..\iostream.h"
#include "LTBDef.h"

FrameData BaseAnim::InterpolateFrame(unsigned int & currentFrame, const AnimNode & Anim, const vector<AnimKeyFrame>& KeyFrames)
{
	int frame0 = 0;
	int frame1 = -1;
	float t = 0.0f;
	for (size_t i = 0; i<KeyFrames.size(); i++)
	{
		if (currentFrame <= KeyFrames[i].m_Time)
		{
			frame1 = i;
			if(i) frame0 = i - 1;
			break;
		}
	}

	if (frame1 == -1)
	{
		currentFrame = 0;
		frame1 = 0;
		t = 0.0f;
	}
	else if (frame0 == frame1) t = 0.0f;
	else t = (float)(currentFrame - KeyFrames[frame0].m_Time) / (float)(KeyFrames[frame1].m_Time - KeyFrames[frame0].m_Time);

	//std::cout<<"curentframe: " << currentFrame<<" t = " << t << " frame0 " << Anim.m_KeyFrames[frame0].m_Time << " frame1 "<< Anim.m_KeyFrames[frame1].m_Time << std::endl;
	if (t == 0) return Anim.Data[frame0];
	if (t == 1) return	Anim.Data[frame1];
	FrameData frame;
	frame.m_Pos = Math::lerp(Anim.Data[frame0].m_Pos, Anim.Data[frame1].m_Pos, t);
	frame.m_Ort = Math::slerp(Anim.Data[frame0].m_Ort, Anim.Data[frame1].m_Ort, t);
	return frame;
}

BaseAnim::BaseAnim()
{
	m_bOnAnimation = 1;
	m_fTime = 0;
	m_iCurrentAnim = 0;
	m_iCurrentFrame = 0;
	m_fSpeed = 1.0 / 30.0f;
	m_bCanRelease = true;
	m_fBlendTime = 0.2f;
	m_More.m_bActive = false;
}

BaseAnim::~BaseAnim()
{

	if (!m_bCanRelease) return;
	for (GLuint i = 0; i < m_pAnimList.size(); i++)
		delete m_pAnimList[i];
}

void BaseAnim::SetAnim(int a)
{
	if (a != m_iCurrentAnim)
	{
		m_iCurrentAnim = a;
		//m_pPlayerMotion->m_iCurrentFrame = 0;
		m_bOnAnimation = 2;
		m_fTime = 0.0f;
	}
}

void BaseAnim::Update(float dt, JointList& skeleton)
{
	if (!m_pAnimList.size()) return;
	m_fTime += dt;
	m_iCurrentFrame = (GLuint)(m_fTime * 1000);

	if (m_More.m_bActive)
	{
		m_More.m_fTime += dt;
		m_More.m_iCurrentFrame = (GLuint)(m_More.m_fTime * 1000);
	}

	Animation* anim = m_pAnimList[m_iCurrentAnim];
	if (!m_CurrentFrames.size()) m_CurrentFrames.resize(anim->AnimNodeLists.size());
	if (m_bOnAnimation == 2) // check blending 
	{
		if (m_fTime > m_fBlendTime)
		{
			m_bOnAnimation = 1;
			m_fTime = 0.0f;
			m_iCurrentFrame = 0;
		}
		else
		{
			float t = m_fTime / m_fBlendTime;
			for (GLuint j = 0; j < anim->AnimNodeLists.size(); j++)
			{

				mat4 m_TransformLocal;

				if (m_More.m_bActive && m_WS[0].Blend[j])
				{
					Animation* anim2 = m_pAnimList[m_More.m_iCurrentAnim];
					FrameData frame = InterpolateFrame(m_More.m_iCurrentFrame, anim2->AnimNodeLists[j], anim2->KeyFrames);

					mat4 transform = frame.m_Ort.ToMatrix();
					transform = transform.Inverse();
					transform.Translate(frame.m_Pos);

					if (anim2->AnimNodeLists[j].Parent != -1)
					{
						m_TransformLocal = transform*skeleton[anim2->AnimNodeLists[j].Parent].LocalTransform;
					}
					else m_TransformLocal = transform;
				}
				else
				{
					AnimNode& Anim = anim->AnimNodeLists[j];
					FrameData frame;
					frame.m_Pos = Math::lerp(m_CurrentFrames[j].m_Pos, Anim.Data[0].m_Pos, t);
					frame.m_Ort = Math::slerp(m_CurrentFrames[j].m_Ort, Anim.Data[0].m_Ort, t);

					// Unrotate + translate transform
					// this matrix transfrom local to parent space 
					mat4 transform = frame.m_Ort.ToMatrix();
					transform = transform.Inverse();

					// test transform for woman skeleton
					Vec3 translate = frame.m_Pos;
					//if(m_pNodeLists[j]->m_Name == "M-bone L UpperArm" || m_pNodeLists[j]->m_Name == "M-bone R UpperArm") translate = frame.m_Pos * (3.0f/4.0f);
					transform.Translate(translate);
					if (Anim.Parent != -1)
					{
						m_TransformLocal = transform*skeleton[Anim.Parent].LocalTransform;
					}
					else m_TransformLocal = transform;
				}


				LTAJoint joint;
				//joint.InvBindPose = m_InvBindPose;
				joint.LocalTransform = m_TransformLocal;
				//joint.parent = m_pParent->m_Index;

				skeleton[j] = joint;
			}
			if (m_More.m_iCurrentFrame == 0) m_More.m_bActive = 0;
		}
		
	}
	else if (m_bOnAnimation == 1)
	{
		for (GLuint i = 0; i < anim->AnimNodeLists.size(); i++)
		{
			mat4 m_TransformLocal;
			

			if (m_More.m_bActive && m_WS[0].Blend[i])
			{
				Animation* anim2 = m_pAnimList[m_More.m_iCurrentAnim];
				FrameData frame = InterpolateFrame(m_More.m_iCurrentFrame, anim2->AnimNodeLists[i], anim2->KeyFrames);

				mat4 transform = frame.m_Ort.ToMatrix();
				transform = transform.Inverse();
				transform.Translate(frame.m_Pos);
		
				if (anim2->AnimNodeLists[i].Parent != -1)
				{
					m_TransformLocal = transform*skeleton[anim2->AnimNodeLists[i].Parent].LocalTransform;
				}
				else m_TransformLocal = transform;
			}
			else
			{
				m_CurrentFrames[i] = InterpolateFrame(m_iCurrentFrame, anim->AnimNodeLists[i], anim->KeyFrames);
				// Unrotate + translate transform
				// this matrix transfrom local to parent space 
				mat4 transform = m_CurrentFrames[i].m_Ort.ToMatrix();
				transform = transform.Inverse();
				transform.Translate(m_CurrentFrames[i].m_Pos);
				//m_TransformLocal = transform;
				
				if (anim->AnimNodeLists[i].Parent != -1)
				{
					m_TransformLocal = transform*skeleton[anim->AnimNodeLists[i].Parent].LocalTransform;
				}
				else m_TransformLocal = transform;
			}
			LTAJoint joint;
			//joint.InvBindPose = m_InvBindPose;
			joint.LocalTransform = m_TransformLocal;
			//joint.parent = m_pParent->m_Index;

			skeleton[i] = joint;
		}

		if (m_iCurrentFrame == 0)	m_fTime = 0;
		if (m_More.m_iCurrentFrame == 0) m_More.m_bActive = 0;
	}
}

void BaseAnim::AddAnimation(GLuint a)
{
	if (m_More.m_bActive) return;
	m_More.m_fTime = 0.0f;
	m_More.m_iCurrentAnim = a;
	m_More.m_iCurrentFrame = 0;
	m_More.m_bActive = 1;
}




FrameData SkeletonNode::InterpolateFrame(unsigned int & currentFrame, const NodeAnim & Anim)
{
	int frame0 = 0;
	int frame1 = -1;
	float t = 0.0f;
	for (size_t i = 0; i<Anim.m_KeyFrames.size(); i++)
	{
		if (currentFrame <= Anim.m_KeyFrames[i].m_Time)
		{
			frame1 = i;
			if (i) frame0 = i - 1;
			break;
		}
	}
	if (frame1 == -1)
	{
		currentFrame = 0;
		frame1 = 0;
		t = 0.0f;
	}
	else if (frame0 == frame1) t = 0.0f;
	else t = (float)(currentFrame - Anim.m_KeyFrames[frame0].m_Time) / (float)(Anim.m_KeyFrames[frame1].m_Time - Anim.m_KeyFrames[frame0].m_Time);

	//std::cout<<"curentframe: " << currentFrame<<" t = " << t << " frame0 " << Anim.m_KeyFrames[frame0].m_Time << " frame1 "<< Anim.m_KeyFrames[frame1].m_Time << std::endl;
	if (t == 0) return Anim.m_Frames[frame0];
	if (t == 1) return	Anim.m_Frames[frame1];
	FrameData frame;
	frame.m_Pos = Math::lerp(Anim.m_Frames[frame0].m_Pos, Anim.m_Frames[frame1].m_Pos, t);
	frame.m_Ort = Math::slerp(Anim.m_Frames[frame0].m_Ort, Anim.m_Frames[frame1].m_Ort, t);
	return frame;
}
SkeletonNode::SkeletonNode()
{
	m_pParent = NULL;
}

SkeletonNode::~SkeletonNode()
{
	for (size_t i = 0; i < m_pChildren.size(); i++)
		delete m_pChildren[i];
}

void SkeletonNode::Update(unsigned int& iframe, int anim, JointList & skeleton)
{
	FrameData frame = InterpolateFrame(iframe, m_Anims[anim]);

	// Unrotate + translate transform
	// this matrix transfrom local to parent space 
	mat4 transform = frame.m_Ort.ToMatrix();
	transform = transform.Inverse();
	transform.Translate(frame.m_Pos);
	//m_TransformLocal = transform;

	if (m_pParent)
	{
		m_TransformLocal = transform*m_pParent->m_TransformLocal;
	}
	else m_TransformLocal = transform;



	LTAJoint joint;
	//joint.InvBindPose = m_InvBindPose;
	joint.LocalTransform = m_TransformLocal;
	//joint.parent = m_pParent->m_Index;

	skeleton.push_back(joint);
	for (size_t i = 0; i<m_pChildren.size(); i++)
	{
		m_pChildren[i]->Update(iframe, anim, skeleton);
	}
}

bool SkeletonNode::LoadAnimation(FILE * pFile, char * AnimName, const std::vector<AnimKeyFrame>& m_KeyFrames, uint32 compresstype)
{
	NodeAnim anim;
	anim.m_NameAnimation = AnimName;
	anim.m_KeyFrames = m_KeyFrames;

	FrameData frame;
	if (compresstype == NONE)
	{
		uint8 bIsVertexAnim;
		fread(&bIsVertexAnim, sizeof(uint8), 1, pFile);
		if (bIsVertexAnim)
		{

		}
		else
		{
			float pos[3];
			float ort[4];
			for (uint16 i = 0; i < m_KeyFrames.size(); i++)
			{
				fread(pos, sizeof(pos), 1, pFile);
				frame.m_Pos.set(pos[0], pos[1], pos[2]);
				anim.m_Frames.push_back(frame);
			}
			for (uint16 i = 0; i < m_KeyFrames.size(); i++)
			{
				fread(ort, sizeof(ort), 1, pFile);
				anim.m_Frames[i].m_Ort.set(ort[3], ort[0], ort[1], ort[2]);
			}
		}
	}
	else if (compresstype == RELEVANT_16)
	{
		uint32 num_keyframes;
		fread(&num_keyframes, sizeof(uint32), 1, pFile);
		int16 compressed_pos[3];
		int16 compressed_rot[4];

		for (uint16 i = 0; i < num_keyframes; i++)
		{
			fread(compressed_pos, sizeof(int16) * 3, 1, pFile);

			frame.m_Pos.set(compressed_pos[0] / 16.0f, compressed_pos[1] / 16.0f, compressed_pos[2] / 16.0f);
			anim.m_Frames.push_back(frame);

		}
		for (uint16 i = num_keyframes; i<m_KeyFrames.size(); i++)
		{
			anim.m_Frames.push_back(anim.m_Frames[0]);
		}
		fread(&num_keyframes, sizeof(uint32), 1, pFile);

		for (uint16 i = 0; i < num_keyframes; i++)
		{
			fread(compressed_rot, sizeof(int16) * 4, 1, pFile);
			anim.m_Frames[i].m_Ort.w = (float)compressed_rot[3] / 0x7FFF;
			anim.m_Frames[i].m_Ort.x = (float)compressed_rot[0] / 0x7FFF;
			anim.m_Frames[i].m_Ort.y = (float)compressed_rot[1] / 0x7FFF;
			anim.m_Frames[i].m_Ort.z = (float)compressed_rot[2] / 0x7FFF;
		}
		for (uint16 i = num_keyframes; i<m_KeyFrames.size(); i++)
		{
			anim.m_Frames[i].m_Ort = anim.m_Frames[0].m_Ort;
		}
	}

	m_Anims.push_back(anim);
	for (uint32 i = 0; i < m_pChildren.size(); i++)
	{
		if (!m_pChildren[i]->LoadAnimation(pFile, AnimName, m_KeyFrames, compresstype))
			continue;
	}

	return true;
}

bool SkeletonNode::LoadSkeleton(FILE * pFile)
{
	uint16 len;
	fread((void*)&len, sizeof(unsigned short), 1, pFile);
	char str[100];
	fread(str, len, 1, pFile);
	str[len] = '\0';

	m_Name = str;

	uint8 m_Flags;
	fread(&m_Index, sizeof(uint16), 1, pFile);
	fread(&m_Flags, sizeof(uint8), 1, pFile);

	float* m = m_GlobalTransform.ToFloatPtr();


	for (int i = 0; i<16; i++) fread(&m[i], sizeof(float), 1, pFile);
	Vec3 v;
	v.set(m_GlobalTransform[3], m_GlobalTransform[7], m_GlobalTransform[11]);
	m_GlobalTransform = m_GlobalTransform.Transpose();
	m_GlobalTransform[3] = m_GlobalTransform[7] = m_GlobalTransform[11] = 0.0f;
	m_GlobalTransform.Translate(v);

	m_InvBindPose = m_GlobalTransform.Inverse();
	uint32 numChild;
	fread(&numChild, sizeof(uint32), 1, pFile);

	for (size_t i = 0; i < numChild; i++)
	{
		SkeletonNode* pNode = new SkeletonNode;
		pNode->m_pParent = this;
		m_pChildren.push_back(pNode);
		pNode->LoadSkeleton(pFile);

	}

	return true;
}
/*

bool BaseAnim::LoadAnimation(FILE * pFile, const vector<SkeNode*>& pNodeList)
{
uint32 CalcNumParentAnims;
unsigned short str_len = 0;
fread(&CalcNumParentAnims, sizeof(uint32), 1, pFile);
//printf("CalcNumParentAnims: %d\n", CalcNumParentAnims);
m_pAnimList.resize(CalcNumParentAnims);
for (uint32 i = 0; i < CalcNumParentAnims; i++)
{
Animation* p = new Animation;
float D[3];
fread(&D, sizeof(float) * 3, 1, pFile);
//printf("Dimension: %.2f, %.2f, %.2f\n",D[0],D[1],D[2]);

// Read name of animation
fread((void*)&str_len, sizeof(unsigned short), 1, pFile);
char str[100];
fread(str, str_len, 1, pFile);
str[str_len] = '\0';

p->Name = str;

fread(&m_CompressionType, sizeof(uint32), 1, pFile);
fread(&m_InterpolationMS, sizeof(uint32), 1, pFile);



uint32 m_KeyFramesGetSize;
fread(&m_KeyFramesGetSize, sizeof(uint32), 1, pFile);
//std::vector<AnimKeyFrame> KeyFrame;
for (uint16 j = 0; j<m_KeyFramesGetSize; j++)
{
AnimKeyFrame key;
fread(&key.m_Time, sizeof(uint32), 1, pFile);
fread((void*)&str_len, sizeof(unsigned short), 1, pFile);
char str1[100];
fread(str1, str_len, 1, pFile);
str1[str_len] = '\0';

key.m_pString = str1;
p->KeyFrames.push_back(key);
}
//m_pSkeleton->LoadAnimation(pFile, str, KeyFrame, m_CompressionType);
//printf("Finish animation: %s\n",str);

for (GLuint j = 0; j < pNodeList.size(); j++)
{
//p->AnimNodeLists.resize(pNodeList.size());
AnimNode node;
node.Parent = pNodeList[j]->m_ParentIndex;
ReadData(pFile, node,p->KeyFrames);
p->AnimNodeLists.push_back(node);
}
m_pAnimList[i] = p;
}
}

void BaseAnim::ReadData(FILE * pFile, AnimNode& node,const vector<AnimKeyFrame>& KeyFrames)
{
// read one animation in file
//for (GLuint k = 0; k < p->AnimNodeLists.size(); k++)
//{
//AnimNode node;
FrameData frame;
if (m_CompressionType == NONE)
{
uint8 bIsVertexAnim;
fread(&bIsVertexAnim, sizeof(uint8), 1, pFile);
if (bIsVertexAnim)
{

}
else
{
float pos[3];
float ort[4];
for (uint16 i = 0; i < KeyFrames.size(); i++)
{
fread(pos, sizeof(pos), 1, pFile);
frame.m_Pos.set(pos[0], pos[1], pos[2]);
node.Data.push_back(frame);
}
for (uint16 i = 0; i < KeyFrames.size(); i++)
{
fread(ort, sizeof(ort), 1, pFile);
node.Data[i].m_Ort.set(ort[3], ort[0], ort[1], ort[2]);
}
}
}
else if (m_CompressionType == RELEVANT_16)
{
uint32 num_keyframes;
fread(&num_keyframes, sizeof(uint32), 1, pFile);
int16 compressed_pos[3];
int16 compressed_rot[4];

for (uint16 i = 0; i < num_keyframes; i++)
{
fread(compressed_pos, sizeof(int16) * 3, 1, pFile);

frame.m_Pos.set(compressed_pos[0] / 16.0f, compressed_pos[1] / 16.0f, compressed_pos[2] / 16.0f);
node.Data.push_back(frame);

}
for (uint16 i = num_keyframes; i < KeyFrames.size(); i++)
{
node.Data.push_back(node.Data[0]);
}
fread(&num_keyframes, sizeof(uint32), 1, pFile);

for (uint16 i = 0; i < num_keyframes; i++)
{
fread(compressed_rot, sizeof(int16) * 4, 1, pFile);
node.Data[i].m_Ort.w = (float)compressed_rot[3] / 0x7FFF;
node.Data[i].m_Ort.x = (float)compressed_rot[0] / 0x7FFF;
node.Data[i].m_Ort.y = (float)compressed_rot[1] / 0x7FFF;
node.Data[i].m_Ort.z = (float)compressed_rot[2] / 0x7FFF;
}
for (uint16 i = num_keyframes; i < KeyFrames.size(); i++)
{
node.Data[i].m_Ort = node.Data[0].m_Ort;
}
}

//p->AnimNodeLists.push_back(node);
//}

//	m_Anims.push_back(anim);
//	for (uint32 i = 0; i < m_pChildren.size(); i++)
//	{
//		if (!m_pChildren[i]->LoadAnimation(pFile, AnimName, m_KeyFrames, compresstype))
//			continue;
//	}

}
*/

