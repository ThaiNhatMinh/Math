#define _CRT_SECURE_NO_WARNINGS
#include "LTAAnimation.h"
#include "gl\glut.h"

char buffer[500];
char name[50];
float x,y,z,w;
Vec3 v,vx,vy,vz;
Quat quat;
FrameData frame;

LTAAnimation::LTAAnimation()
{
	m_iNumJoints = 0;
	m_fAnimTime = 0;
	RootNode = new ModelNode;
}


LTAAnimation::~LTAAnimation()
{
	delete RootNode;
}

bool LTAAnimation::LoadHierarchy(const std::string& filename)
{
	
	FILE* pFile = fopen(filename.c_str(),"rt");
	if(!pFile)
	{
		std::cerr << "LTAModel::LoadModel() Failed to open file: " << filename << std::endl;
		return false;
	}
	Load(pFile,RootNode);
	fclose(pFile);
	return true;
}

// recure and load joint node
LTAAnimation::Return_Type LTAAnimation::Load(FILE* pFile,ModelNode* Root)
{		
		
		//count++;
		fgets(buffer,512,pFile); // {children
		//fscanf(pFile,"%[^\n]",buffer);
		if(strstr(buffer,"{children"))
		{
			
			fgets(buffer,512,pFile); // {
			for(;;)
			{
				if(Load(pFile,Root)==CHILDREN) break;
			}

		}
		else if(strstr(buffer,"}"))
		{
			if(Root->m_flag) 
			{
				Root->m_flag = false;
				return TRANSFORM;
			}
			else
			{
				return CHILDREN;
			}


		}
		else if(strstr(buffer,"{transform"))
		{
			//m_Num++;.
			
			strcpy(name,strstr(buffer,"\""));
			int len = strlen(name);
			memmove(name, name+1,len-4);
			name[len-4] = '\0';

			ModelNode* p = new ModelNode;
			assert(p!=NULL);
			fgets(buffer,512,pFile); // {matrix
			fgets(buffer,512,pFile); // 
			fscanf(pFile,"%[\t]{%f%f%f%f}",buffer,&vx.x,&vy.x,&vz.x,&v.x);
			fgets(buffer,12,pFile);
			fscanf(pFile,"%[\t]{%f%f%f%f}",buffer,&vx.y,&vy.y,&vz.y,&v.y);
			fgets(buffer,12,pFile);
			fscanf(pFile,"%[\t]{%f%f%f%f}",buffer,&vx.z,&vy.z,&vz.z,&v.z);
			fgets(buffer,12,pFile);
			fscanf(pFile,"%[\t]{%f%f%f%f}",buffer,&x,&y,&z,&w);
			fgets(buffer,12,pFile);
			
			fgets(buffer,512,pFile); // }
			fgets(buffer,512,pFile); // } from matrix
			Matrix4 M(vx,vy,vz,v);

			p->m_name = name;
			p->m_pParent = Root;
			p->m_DefaultTransform = M;
			//p->m_InvDefaultTransform = M.Inverse();
			p->m_flag = true;
			p->m_index = m_iNumJoints;
			if(!strcmp(name, "Scene Root"))
			{
				Root->m_name = name;
				Root->m_DefaultTransform = M;
				Root->m_flag = true;
				Root->m_index = m_iNumJoints;
				m_iNumJoints = 1;
				delete p;
				Load(pFile,Root);
			}else
			{
				m_iNumJoints++;
				Root->m_Children.push_back(p);
				Load(pFile,p);
			}
			
		}
		Load(pFile,Root);
	return COMPILE;
}

// print hierarchy of skeleton
void LTAAnimation::Printf()
{
	if(RootNode==NULL) return;

	RootNode->Print("-");
}

void LTAAnimation::PrintfFrame(int idframe,ModelNode* pNode)
{
	
	if(pNode==NULL) 
	{
		FrameData& frame = RootNode->m_Anim[0].m_Frames[idframe];
		std::cout <<RootNode->m_name<< " " <<frame.m_Pos << std::endl;

		for(size_t i=0; i<RootNode->m_Children.size(); i++)
			PrintfFrame(idframe, RootNode->m_Children[i]);
	}
	else 
	{
		FrameData& frame = pNode->m_Anim[0].m_Frames[idframe];
		std::cout <<pNode->m_name<< " " <<frame.m_Pos << std::endl;

		for(size_t i=0; i<pNode->m_Children.size(); i++)
			PrintfFrame(idframe, pNode->m_Children[i]);
	}

	


}
// Render skeleton
void LTAAnimation::Render()
{
	glPointSize(5.0f);
	RootNode->Render();
	glPointSize(1.0f);
}

void LTAAnimation::LoadAnimation(FILE* pFile, ModelNode* Root,int numFrame,char* nameAni)
{
	if(Root==NULL) return;
	NodeAnim anim;
	anim.m_NameAnimation = nameAni;
	//anim.m_Frames.push_back
	fgets(buffer,500,pFile); //{anim
	fgets(buffer,500,pFile); //{parent "" }
	fgets(buffer,500,pFile); //{frames
	fgets(buffer,500,pFile); //{posquat
	fgets(buffer,500,pFile); //{
	for(int i= 0; i<numFrame;i++)
	{
		fscanf(pFile,"%[\t]{\t{%f%f%f}\n",buffer,&v.x,&v.y,&v.z);
		fgets(buffer,12,pFile);
		//std::cout << v <<std::endl;
		fscanf(pFile,"%[\t]{%f%f%f%f}\n",buffer,&quat.x,&quat.y,&quat.z,&quat.w);
		fgets(buffer,12,pFile);
		fgets(buffer,500,pFile); //}

		frame.m_Pos = v;
		frame.m_Ort = quat;
		if(!i) std::cout<<Root->m_name << " " << quat<< std::endl;
		anim.m_Frames.push_back(frame);
	}

	fgets(buffer,500,pFile); //}
	fgets(buffer,500,pFile); //}
	fgets(buffer,500,pFile); //}
	fgets(buffer,500,pFile); //}

	Root->m_Anim.push_back(anim);

	for(size_t i=0; i<Root->m_Children.size(); i++)
		LoadAnimation(pFile,Root->m_Children[i],numFrame,nameAni);
}

bool LTAAnimation::LoadAnimation(const std::string& filename)
{
	FILE* pFile = fopen(filename.c_str(),"rt");
	if(!pFile) 
	{
		std::cerr<< "LTAAnimation::LoadAinmation() Failed to open file: "<<filename<< std::endl;
		return false;
	}
	int numframe; 

	while(!feof(pFile))
	{
		fgets(buffer,500,pFile); // {animset ""
		strcpy(name,strstr(buffer,"\""));
		int len = strlen(name);
		memmove(name, name+1,len-4);
		name[len-4] = '\0';

		fscanf(pFile,"%s %d %s\n",buffer,&numframe, buffer); // {num  }
		fgets(buffer,500,pFile); // {anim
		fgets(buffer,500,pFile); // {

		std::cout<< name  << " " <<numframe<< std::endl;
		LoadAnimation(pFile,RootNode,numframe,name);
		m_ListAnimation.push_back(name);
		fgets(buffer,500,pFile); // }
		fgets(buffer,500,pFile); // }
		fgets(buffer,500,pFile); // }
	}

	fclose(pFile);
	return true;
}

void LTAAnimation::Update(float dt)
{
	int currAnim = 0;
	static float time = 0;
	static unsigned int frame = 0;
	time +=dt;
	if(time>0.09f) 
	{
		time =0.0f;
		frame++;
		if(RootNode->m_Anim[currAnim].m_Frames.size() <=frame) frame =0;
	}
	m_fAnimTime +=dt;
	m_AnimatedSkeleton.clear();
	BuildAnimation(RootNode, frame,currAnim);


}

// Calculate pos and orient for this node/joint
void LTAAnimation::BuildAnimation(ModelNode* Root, int frameID, int idAnim)
{
	LTAJoint joint;

	FrameData& frame = Root->m_Anim[idAnim].m_Frames[frameID];

	// Unrotate + translate transform
	// this matrix transfrom local to parent space 
	Mat4 transform = frame.m_Ort.ToMatrix();
	transform = transform.Inverse();
	transform.Translate(frame.m_Pos);
	Root->m_TransformLocal = transform;

	if(Root->m_pParent)
	{
		Root->m_GlobalTransform =   transform*Root->m_pParent->m_GlobalTransform  ;
	}
	else Root->m_GlobalTransform = transform;

	// Calculate final position in model space
	

	joint.InvBindPose = Root->m_DefaultTransform.Inverse();
	joint.LocalTransform = Root->m_GlobalTransform;
	joint.orientation = frame.m_Ort;

	if(Root->m_pParent)
	{
		joint.parent = Root->m_pParent->m_index;
		//joint.LocalTransform = Root->m_pParent->m_GlobalTransform;
	}
	else
	{
		joint.parent = -1;
		//joint.LocalTransform = transform;
	}

	m_AnimatedSkeleton.push_back(joint);

	if(Root->m_pParent) Root->m_FinalPos = Root->m_pParent->m_GlobalTransform * frame.m_Pos;
	else Root->m_FinalPos = transform * frame.m_Pos;
	//std::cout <<Root->m_name<< Root->m_FinalPos <<std::endl;
	for(size_t i=0;i<Root->m_Children.size();i++)
	{
		BuildAnimation(Root->m_Children[i],frameID,idAnim);
	}
}