#include <iostream>
#include <string>
#include <vector>

#include "Math\Vector.h"
#include "Math\Quaternion.h"
#include "Math\Matrix.h"
#include"gl\glut.h"
#pragma once
#ifndef __LTA_ANIMATION__
#define __LTA_ANIMATION__
struct FrameData
{
        Vec3 m_Pos;
		Quat m_Ort;
};

// Animation of a joint;
struct NodeAnim
{
		std::string m_NameAnimation;
		std::vector<FrameData> m_Frames;
};


typedef std::vector<FrameData> FrameDataList;
// All animation in here
typedef std::vector<NodeAnim> AnimationList;

class ModelNode
	{
	public:
		
		ModelNode* m_pParent;
		bool m_flag;
		std::string m_name;
		int m_index;
		Matrix4 m_DefaultTransform;
		Matrix4 m_TransformLocal;
		Matrix4 m_GlobalTransform;
		std::vector<ModelNode*> m_Children;
		AnimationList       m_Anim;
		Vec3 m_FinalPos;

	public:
		ModelNode()
		{
			m_pParent = NULL;
		};
		~ModelNode()
		{
			for(size_t i=0;i < m_Children.size();i++)
				delete m_Children[i];
		};
		void Print(std::string space)
		{
			std::cout<<space<<m_name<<std::endl;
			for(size_t i=0;i < m_Children.size();i++)
				m_Children[i]->Print(space + " ");
		}
		void Render()
		{
			glPushMatrix();
			//glTranslatef(m_transform(0,3),m_transform(1,3),m_transform(2,3));
			//glMultMatrixf(m_Transform.ToFloatPtr());
			glColor3f(0.1f, 0.9f, 0.9f);
			glBegin(GL_POINTS);
			glVertex3f(m_FinalPos.x,m_FinalPos.y,m_FinalPos.z);
			//glVertex3f(m_DefaultTransform(3,0),m_DefaultTransform(3,1),m_DefaultTransform(3,2));
			glEnd();

			glBegin(GL_LINES);
			for(size_t i=0;i<m_Children.size();i++)
			{
				glVertex3f(m_FinalPos.x,m_FinalPos.y,m_FinalPos.z);
				glVertex3f(m_Children[i]->m_FinalPos.x,m_Children[i]->m_FinalPos.y,m_Children[i]->m_FinalPos.z);
				//glVertex3f(m_DefaultTransform(3,0),m_DefaultTransform(3,1),m_DefaultTransform(3,2));
				//glVertex3f(m_Children[i]->m_DefaultTransform(3,0),m_Children[i]->m_DefaultTransform(3,1),m_Children[i]->m_DefaultTransform(3,2));
			}
			glEnd();
			
			for(size_t i=0;i<m_Children.size();i++)
			{
				if(m_Children[i]->m_name=="M-bone L Thigh" || m_Children[i]->m_name=="M-bone R Thigh") continue;
				m_Children[i]->Render();
			}

			glPopMatrix();
		}
	};

class LTAAnimation
{
	enum Return_Type
	{
		COMPILE,
		CHILDREN,
		TRANSFORM,
	};

public:
	
	LTAAnimation();
	~LTAAnimation();
	// Load an animation from the animation file
	bool LoadAnimation( const std::string& filename );
    // Update this animation's joint set.
    void Update( float fDeltaTime );
    // Draw the animated skeleton
    void Render();
	// The JointInfo stores the information necessary to build the
    // skeletons for each frame
    bool LoadHierarchy(const std::string& filename);
	
	struct LTAJoint
	{
		int parent;
		//Vec3 position;
		Quat orientation;
		Mat4 InvBindPose;
		Mat4 LocalTransform;
	};
	struct LTASkeleton{
		int numJoint;
		LTAJoint* joints;
	};
   

	
	typedef std::vector<LTAJoint> JointList;

    
 
    const JointList& GetSkeleton() const
    {
        return m_AnimatedSkeleton;
    }
 
    int GetNumJoints() const
    {
        return m_iNumJoints;
    }
	void Printf();
	void PrintfFrame(int idframe,ModelNode* pNode);
 
protected:
	Return_Type Load(FILE* pFile,ModelNode* Root);
	bool Load(FILE* pFile, ModelNode* Root, bool flag);
	void LoadAnimation(FILE* pFile, ModelNode* Root,int numFrame,char* nameAnimation);

	void BuildAnimation(ModelNode* Root, int frame, int idAnim);
	ModelNode* RootNode;
    JointList       m_AnimatedSkeleton;
 
    // Build the frame skeleton for a particular frame
    //void BuildFrameSkeleton( FrameSkeletonList& skeletons, const JointInfoList& jointInfo, const BaseFrameList& baseFrames, const FrameData& frameData );
    //void InterpolateSkeletons( FrameSkeleton& finalSkeleton, const FrameSkeleton& skeleton0, const FrameSkeleton& skeleton1, float fInterpolate );
 
private:
    int m_iNumFrame;
	int m_iNumJoints;
	bool m_bHasAnimation;
	float m_fAnimTime;
	std::vector<std::string> m_ListAnimation;
};


#endif