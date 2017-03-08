#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "Math\Vector.h"
#include "Math\Quaternion.h"
#include "Math\Matrix.h"
#include <assert.h>
using std::vector;
using std::string;
class MD5Animation
{
public:
	MD5Animation();
	virtual ~MD5Animation();

	bool LoadAnimaion(const string& filename);
	void Update(float dt);
	void Render();

	struct JointInfo
    {
        string		m_Name;
        int         m_ParentID;
        int         m_Flags;
        int         m_StartIndex;
    };
	typedef std::vector<JointInfo> JointInfoList;

	struct Bound
    {
        Vec3   m_Min;
        Vec3   m_Max;
    };
    typedef std::vector<Bound> BoundList;

	struct BaseFrame
    {
        Vec3   m_Pos;
        Quat   m_Orient;
    };
    typedef std::vector<BaseFrame> BaseFrameList;

	struct FrameData
    {
        int m_iFrameID;
        std::vector<float> m_FrameData;
    };
    typedef std::vector<FrameData> FrameDataList;

	// A Skeleton joint is a joint of the skeleton per frame
    struct SkeletonJoint
    {
        SkeletonJoint()
            : m_Parent(-1)
            
        {}
 
        SkeletonJoint( const BaseFrame& copy )
            : m_Pos( copy.m_Pos )
            , m_Orient( copy.m_Orient )
        {}
 
        int         m_Parent;
        Vec3		m_Pos;
		Quat		m_Orient;
    };
    typedef std::vector<SkeletonJoint> SkeletonJointList;


	// A frame skeleton stores the joints of the skeleton for a single frame.
    struct FrameSkeleton
    {
        SkeletonJointList   m_Joints;
    };
    typedef std::vector<FrameSkeleton> FrameSkeletonList;
	const FrameSkeleton& GetSkeleton() const
    {
        return m_AnimatedSkeleton;
    }
 
    int GetNumJoints() const
    {
        return m_iNumJoints;
    }
 
    const JointInfo& GetJointInfo( unsigned int index ) const
    {
        assert( index < m_JointInfos.size() );
        return m_JointInfos[index];
    }

protected:
	JointInfoList       m_JointInfos;
    BoundList           m_Bounds;
    BaseFrameList       m_BaseFrames;
    FrameDataList       m_Frames;
    FrameSkeletonList   m_Skeletons;    // All the skeletons for all the frames
 
    FrameSkeleton       m_AnimatedSkeleton;
 
    // Build the frame skeleton for a particular frame
    void BuildFrameSkeleton( FrameSkeletonList& skeletons, const JointInfoList& jointInfo, const BaseFrameList& baseFrames, const FrameData& frameData );
    void InterpolateSkeletons( FrameSkeleton& finalSkeleton, const FrameSkeleton& skeleton0, const FrameSkeleton& skeleton1, float fInterpolate );

private:
    int m_iMD5Version;
    int m_iNumFrames;
    int m_iNumJoints;
    int m_iFramRate;
    int m_iNumAnimatedComponents;
 
    float m_fAnimDuration;
    float m_fFrameDuration;
    float m_fAnimTime;
};

