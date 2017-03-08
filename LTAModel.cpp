#define _CRT_SECURE_NO_WARNINGS
#include "LTAModel.h"
#include <stdio.h>
#include "Math\CMath.h"
#include "gl\glut.h"
bool LTAModel::LoadModel(const std::string& filename)
{
	FILE* pFile = fopen(filename.c_str(), "rt");
	if(!pFile) 
	{
		std::cerr << "LTAModel::LoadModel() Failed to open file: " << filename << std::endl;
		return false;
	}

	char buff[30];int num = 0;
	fscanf(pFile,"%s %d\n",buff,&num);
	for(int i=0; i<num; i++)
	{
		fgets(buff,30,pFile);
		int len = strlen(buff);
		buff[len-1] = '\0';
		m_NodeList.push_back(buff);
	}
	while(!feof(pFile))
	{
		Mesh mesh;
		
		std::vector<Vector3> m_Vertex, m_Normal;
		std::vector<Triangle> m_Index;
		char buff[1024];
		float x,y,z;
		int a,b,c;

		fgets(buff,50,pFile); // shape Gun
		int len = strlen(buff);
		buff[len-1] = '\0';

		mesh.m_name = buff;
		//std::cout << buff << std::endl;
		//mesh.m_name = mesh.m_name.substr(6,strlen(buff)-1);

		fgets(buff,21,pFile); // {
		fgets(buff,21,pFile); // vertex
		fgets(buff,21,pFile); // {
		while(fscanf(pFile,"%f %f %f",&x,&y,&z))	m_Vertex.push_back(Vector3(x,y,z));
		fgets(buff,21,pFile); // }
		fgets(buff,21,pFile); // normal
		fgets(buff,21,pFile); // {
		while(fscanf(pFile,"%f %f %f",&x,&y,&z))	m_Normal.push_back(Vector3(x,y,z));
		fgets(buff,21,pFile); // }
		fgets(buff,21,pFile); // index
		fgets(buff,21,pFile); // {
		while (fscanf(pFile,"%d %d %d",&a,&b,&c))
		{
			Triangle tri(a,b,c);
			mesh.m_Tris.push_back(tri);
			mesh.m_IndexBuffer.push_back( tri.m_Indices[0] );
			mesh.m_IndexBuffer.push_back( tri.m_Indices[1] );
			mesh.m_IndexBuffer.push_back( tri.m_Indices[2] );
		}
		fgets(buff,21,pFile); // }
		fgets(buff,21,pFile); // weightsets
		fgets(buff,21,pFile); // {
		for(int i=1; i!=m_Vertex.size()+1;i++)
		{
			a=b= -1;
			x=y= -1.0f;
			char s1,s2;
			if(fscanf(pFile,"\t\t%c %d %f %d %f %c\n",&s1,&a,&x,&b,&y,&s2)==3) fscanf(pFile, "%c%c",&s1,&s2);
			Weight weight;
			weight.m_JointID[0] = a;
			weight.m_JointID[1] = b;
			weight.m_Bias[0] = x;
			weight.m_Bias[1] = y;
			mesh.m_Weights.push_back(weight);
		}
		fgets(buff,21,pFile); // }
		fgets(buff,21,pFile); // }

		for(size_t i=0; i<m_Vertex.size(); i++)
		{
			Vertex vert;
			vert.m_Normal = m_Normal[i];
			vert.m_Pos = m_Vertex[i];
			mesh.m_Verts.push_back(vert);
		}

		PrepareMesh(mesh);
        PrepareNormals(mesh);
		m_Meshes.push_back(mesh);
	}

	fclose(pFile);

	return true;
}
/*
bool LTAModel::LoadHierarchy(const std::string& filename)
{
	FILE* pFile = fopen("Hierarchy.txt","rt");
	if(!pFile)
	{
		std::cerr << "LTAModel::LoadModel() Failed to open file: " << filename << std::endl;
		return false;
	}
	Load(pFile,-1);
	fclose(pFile);
}
LTAModel::Return_Type LTAModel::Load(FILE* pFile, int parentID)
{		
		char buffer[500];
		char name[25];
		float x,y,z,w;
		vec3 v,vx,vy,vz;
		//count++;
		fgets(buffer,512,pFile); // {children
		//fscanf(pFile,"%[^\n]",buffer);
		if(strstr(buffer,"{children"))
		{
			
			fgets(buffer,512,pFile); // {
			for(;;)
			{
				if(Load(pFile,parentID)==CHILDREN) break;
			}

		}
		else if(strstr(buffer,"}"))
		{
			if(m_Joints[parentID].m_flag) 
			{
				m_Joints[parentID].m_flag = false;
				return TRANSFORM;
			}
			else
			{
				return CHILDREN;
			}


		}
		else if(strstr(buffer,"{transform"))
		{
			//m_Num++;
			strcpy(name,strstr(buffer,"\""));
			int len = strlen(name);
			memmove(name, name+1,len-4);
			name[len-4] = '\0';
			//Node* p = Make_Node();
			//p->IsOpen = true;
			Joint joint;
			fgets(buffer,512,pFile); // {matrix
			fgets(buffer,512,pFile); // 
			fscanf(pFile,"%[\t]{%f%f%f%f}",buffer,&vx.x,&vy.x,&vz.x,&v.x);
			fgets(buffer,12,pFile);
			fscanf(pFile,"%[\t]{%f%f%f%f}",buffer,&vx.y,&vy.y,&vz.y,&v.y);
			fgets(buffer,12,pFile);
			fscanf(pFile,"%[\t]{%f%f%f%f}",buffer,&vx.z,&vy.z,&vz.z,&v.z);
			fgets(buffer,12,pFile);
			fscanf(pFile,"%[\t]{%f%f%f%f}",buffer,&x,&y,&z,&x);
			fgets(buffer,12,pFile);
			
			fgets(buffer,512,pFile); // }
			fgets(buffer,512,pFile); // } from matrix
			Matrix4 M(vx,vy,vz);

			joint.m_Pos = v;
			joint.m_Name = name;
			joint.m_ParentID = parentID;
			joint.m_Orient = M.ToQuat();
			m_Joints.push_back(joint);
			joint.m_flag = true;
			//p->m_Pos = v;
			//cout<<v.x<<' '<<v.y<<' '<< v.z<<endl;
			//p->m_Name = name;
			//p->m_Parent = Root;
			//p->m_ID = m_Num;
			//Root->m_Children.push_back(p);
			
			Load(pFile,FindID(name));
		}
		Load(pFile,parentID);
		//fgets(buffer,512,pFile); 
		//if(Root->IsOpen) Root->IsOpen = false;
}
*/
int LTAModel::FindID(std::string name)
{
	for(size_t i=0; i<m_NodeList.size(); i++)
		if(m_NodeList[i]==name) return i;
	return -1;
}

bool  LTAModel::PrepareMesh(Mesh& mesh)
{
	
    mesh.m_PositionBuffer.clear();
    //mesh.m_Tex2DBuffer.clear();
 
    // Compute vertex positions
    for ( unsigned int i = 0; i < mesh.m_Verts.size(); ++i )
    {
		Vertex& vert = mesh.m_Verts[i];
		mesh.m_PositionBuffer.push_back(vert.m_Pos);
		/*
		vert.m_Pos.zero();
        vert.m_Normal.zero();
		for(int j=0; j<2; i++)
		{
			
			Weight& weight = mesh.m_Weights[i];
			if(weight.m_JointID[j]==-1) break;
			Joint& joint = m_Joints[weight.m_JointID[j]];
			//vec3 rotpos = joint.m_Orient * weight.
			vert.m_Pos += (joint.m_Pos /*  ) * weight.m_Bias[j];
		}
		*/
        //mesh.m_Tex2DBuffer.push_back(vert.m_Tex0);
	}
	return true;

}
bool LTAModel::PrepareNormals( Mesh& mesh )
{
	for ( unsigned int i = 0; i < mesh.m_Verts.size(); ++i )
    {
		Vertex& vert = mesh.m_Verts[i];
		vec3 normal = Math::Normalize( vert.m_Normal );

		 mesh.m_NormalBuffer.push_back( normal );
		 /*
		 vert.m_Normal.zero();
		 for ( int j = 0; j < 2; ++j )
        {
            const Weight& weight = mesh.m_Weights[i];
			if(weight.m_JointID[j]==-1) break;
            const Joint& joint = m_Joints[weight.m_JointID[j]];
            vert.m_Normal += ( normal * joint.m_Orient ) * weight.m_Bias[j];
        }
		*/
	}

	return true;
}

void LTAModel::Render()
{
    glPushMatrix();
	//m_LocalToWorldMatrix.Translate(0,0,50);
   // glMultMatrixf( m_LocalToWorldMatrix.ToFloatPtr() );
 
    // Render the meshes
    for ( unsigned int i = 0; i < m_Meshes.size(); ++i )
    {
        RenderMesh( m_Meshes[i] );
    }
 
    //m_Animation.Render();
 
    for ( unsigned int i = 0; i < m_Meshes.size(); ++i )
    {
        //RenderNormals( m_Meshes[i] );
    }
 
    glPopMatrix();
}

void LTAModel::RenderMesh( const Mesh& mesh )
{
    glColor3f( 1.0f, 1.0f, 1.0f );
   
	glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    glEnableClientState( GL_NORMAL_ARRAY );
    //glBindTexture( GL_TEXTURE_2D, mesh.m_TexID );
    glVertexPointer( 3, GL_FLOAT, 0, &(mesh.m_PositionBuffer[0]) );
    glNormalPointer( GL_FLOAT, 0, &(mesh.m_NormalBuffer[0]) );
   // glTexCoordPointer( 2, GL_FLOAT, 0, &(mesh.m_Tex2DBuffer[0]) );
 
    glDrawElements( GL_TRIANGLES, mesh.m_IndexBuffer.size(), GL_UNSIGNED_INT, &(mesh.m_IndexBuffer[0]) );
 
    glDisableClientState( GL_NORMAL_ARRAY );
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    glDisableClientState( GL_VERTEX_ARRAY );
 
    glBindTexture( GL_TEXTURE_2D, 0 );
}

void LTAModel::RenderNormals(const Mesh& mesh)
{
	glPushAttrib( GL_ENABLE_BIT );
    glDisable( GL_LIGHTING );

    glColor3f( 1.0f, 1.0f, 0.0f );// Yellow

    glBegin( GL_LINES );
    {
        for ( unsigned int i = 0; i < mesh.m_PositionBuffer.size(); ++i )
        {
            Vec3 p0 = mesh.m_PositionBuffer[i];
            Vec3 p1 = ( mesh.m_PositionBuffer[i] + mesh.m_NormalBuffer[i] );

            glVertex3f(p0.x,p0.y,p0.z);
            glVertex3f(p1.x,p1.y,p1.z );
        }
    }
    glEnd();

}
void LTAModel::DebugMesh(int id)
{
	for(size_t i=0; i<m_Meshes[id].m_PositionBuffer.size(); i++)
	{
		const Vertex& vert = m_Meshes[id].m_Verts[i];
		std::cout << vert.m_Pos.x << " , " << vert.m_Pos.y << " , " << vert.m_Pos.z << std::endl;
	}
}

void LTAModel::DebugWeight(int id)
{
	for(size_t i=0; i<m_Meshes[id].m_Weights.size(); i++)
	{
		const Weight& weight = m_Meshes[id].m_Weights[i];
		std::cout << weight.m_JointID[0]<<' ' << weight.m_Bias[0];
		if (weight.m_JointID[1]!=-1)
		{
			std::cout << ' '<<weight.m_JointID[1]<< ' ' << weight.m_Bias[1]<<std::endl;
		}
		else std::cout <<'\n';
	}
}

void LTAModel::Update(const LTAAnimation::JointList& skeleton)
{
	
	for(size_t i=0; i<0;i++)
	{
		Mesh& mesh = m_Meshes[i];
		//if(mesh.m_name=="shape 416C_body" || mesh.m_name=="shape 416C_reload" || mesh.m_name=="shape 416C_cartridge") continue;
		for(size_t j=0; j<m_Meshes[i].m_Verts.size(); j++)
		{

			const Vertex& vert =mesh.m_Verts[j];
			const Weight& weight = mesh.m_Weights[j];
			Vec3& pos = mesh.m_PositionBuffer[j];
			Vec3& normal = mesh.m_NormalBuffer[j];
 
			pos.zero();
			normal.zero();
			int id = weight.m_JointID[0];
			int parentID = skeleton[id].parent;
			//pos = (skeleton[id].InvBindPose *skeleton[id].LocalTransform)*vert.m_Pos * weight.m_Bias[0] ;
			
			Vec4 temp(vert.m_Pos, 1.0f);
			
			temp = (skeleton[id].InvBindPose *skeleton[id].LocalTransform)*temp * weight.m_Bias[0];
			
			pos = Vector3(temp.x ,temp.y,temp.z) ;
			
			normal = skeleton[id].orientation * vert.m_Normal * weight.m_Bias[0];
			id = weight.m_JointID[1];
			if(id!=-1){
				
				Vec4 temp2(vert.m_Pos, 1.0f);
				temp2 = (skeleton[id].InvBindPose *skeleton[id].LocalTransform)*temp2 * weight.m_Bias[1];
				pos += Vector3(temp2.x,temp2.y,temp2.z) ;
				
				//pos += (skeleton[id].InvBindPose *skeleton[id].LocalTransform)*vert.m_Pos * weight.m_Bias[1];
				normal += skeleton[id].orientation * vert.m_Normal * weight.m_Bias[1];
			}
		}
    }
}
