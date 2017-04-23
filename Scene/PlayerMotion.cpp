#include "..\iostream.h"

PlayerMotion::~PlayerMotion()
{
	for (GLuint i = 0; i < m_pProp->m_nNodes; i++)
		delete m_pNodeLists[i];

	for (GLuint i = 0; i < m_pProp->m_nPieces; i++)
		delete m_pMeshs[i];

	for (GLuint i = 0; i < m_pProp->m_nParentAnims; i++)
		delete m_pAnimList[i];
	//delete m_pAnimation;
	delete m_pProp;

}

void PlayerMotion::Init()
{
	if (LTBFile::BeginLoad("Game\\Model\\CHARACTER\\M-MOTION.LTB"))
	{
		m_pProp =		LTBFile::LoadProp();
		m_pMeshs =		LTBFile::LoadMesh();
		m_pNodeLists =	LTBFile::LoadSkeleton();
						LTBFile::LoadWS();
		m_ChildModel = 	LTBFile::LoadChildName();
		m_pAnimList =	LTBFile::LoadAnimation(m_pNodeLists);
		m_Socket = 		LTBFile::LoadSocket();
						LTBFile::EndLoad();
	}
	else
	{
		Log::Message(Log::LOG_ERROR, "Can't open file motion player.");
	}
}

void PlayerMotion::SetData(BaseAnim * p)
{
	p->m_bCanRelease = false;
	p->m_pAnimList = m_pAnimList;
}


/*
void PlayerMotion::Update(float dt)
{
	for (size_t i = 0; i < m_MotionRegister.size(); i++)
	{
		
		MotionRegister* mr = m_MotionRegister[i];
		
		mr->m_fTime += dt;
		
		
		if (mr->m_iCurrentAnim >= m_pProp->m_nParentAnims) mr->m_iCurrentAnim = 0;

		Animation* anim = m_pAnimList[mr->m_iCurrentAnim];
		JointList& skeleton = mr->m_This->jointlist;
		

		if (mr->m_bOnAnimation == 2) // check blending 
		{
			if (mr->m_fTime > mr->m_fBlendTime)
			{
				mr->m_bOnAnimation = 1;
				mr->m_fTime = 0.0f;
				mr->m_iCurrentFrame = 0;
			}
			else
			{
				float t = mr->m_fTime / mr->m_fBlendTime;
				for (GLuint j = 0; j < m_pProp->m_nNodes; j++)
				{
					AnimNode& Anim = anim->AnimNodeLists[j];
					FrameData frame;
					frame.m_Pos = Math::lerp(mr->m_Frames[j].m_Pos, Anim.Data[0].m_Pos, t);
					frame.m_Ort = Math::slerp(mr->m_Frames[j].m_Ort, Anim.Data[0].m_Ort, t);

					// Unrotate + translate transform
					// this matrix transfrom local to parent space 
					mat4 transform = frame.m_Ort.ToMatrix();
					transform = transform.Inverse();

					// test transform for woman skeleton
					Vec3 translate = frame.m_Pos;
					//if(m_pNodeLists[j]->m_Name == "M-bone L UpperArm" || m_pNodeLists[j]->m_Name == "M-bone R UpperArm") translate = frame.m_Pos * (3.0f/4.0f);
					transform.Translate(translate);
					//m_TransformLocal = transform;
					mat4 m_TransformLocal;
					if (Anim.Parent != -1)
					{
						m_TransformLocal = transform*skeleton[Anim.Parent].LocalTransform;
					}
					else m_TransformLocal = transform;



					LTAJoint joint;
					//joint.InvBindPose = m_InvBindPose;
					joint.LocalTransform = m_TransformLocal;
					//joint.parent = m_pParent->m_Index;

					skeleton[j] = joint;
				}
			}
		}
		else if(mr->m_bOnAnimation == 1)
		{
			//cout << mr->m_iCurrentFrame << endl;
			mr->m_iCurrentFrame = (GLuint)(mr->m_fTime * 1000);


			for (GLuint j = 0; j < m_pProp->m_nNodes; j++)
			{

				mr->m_Frames[j] = SkeAnim::InterpolateFrame(mr->m_iCurrentFrame, anim->AnimNodeLists[j], anim->KeyFrames);

				// Unrotate + translate transform
				// this matrix transfrom local to parent space 
				mat4 transform = mr->m_Frames[j].m_Ort.ToMatrix();
				transform = transform.Inverse();

				// test transform for woman skeleton
				Vec3 translate = mr->m_Frames[j].m_Pos;
				//if(m_pNodeLists[j]->m_Name == "M-bone L UpperArm" || m_pNodeLists[j]->m_Name == "M-bone R UpperArm") translate = frame.m_Pos * (3.0f/4.0f);
				transform.Translate(translate);
				//m_TransformLocal = transform;
				mat4 m_TransformLocal;
				if (anim->AnimNodeLists[j].Parent != -1)
				{
					m_TransformLocal = transform*skeleton[anim->AnimNodeLists[j].Parent].LocalTransform;
				}
				else m_TransformLocal = transform;



				LTAJoint joint;
				//joint.InvBindPose = m_InvBindPose;
				joint.LocalTransform = m_TransformLocal;
				//joint.parent = m_pParent->m_Index;

				skeleton[j] = joint;
			}
			
			if (mr->m_iCurrentFrame == 0)
			{
				mr->m_fTime = 0;
				//if (mr->m_iCurrentAnim != sniper + idle)
				//{
				//	mr->m_iCurrentAnim = sniper;
				//	mr->m_bOnAnimation = 1;
				//}
			}
		}
	}
}
*/
/*
void PlayerMotion::AddPlayer(MotionRegister * pMr)
{
	pMr->m_Frames.resize(m_pProp->m_nNodes);
	m_MotionRegister.push_back(pMr);
}
*/