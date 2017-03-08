#include "MD5Model.h"
#include <boost\filesystem.hpp>
#include <assert.h>
#include <SOIL.h>
#include "Math\CMath.h"
#include "gl\glut.h"
namespace fs = boost::filesystem;
using std::cout;
using std::cerr;


MD5Model::MD5Model()
{
}


MD5Model::~MD5Model()
{
}


bool MD5Model::LoadAnim(const string& filename)
{
	if( !fs::exists(filename)) 
	{
		cerr<< "MD5Model::LoadModel() Fail to find file:" << filename<< std::endl;
		return false;
	}

	fs::path filePath = filename;
	fs::path parentPath = filePath.parent_path();
	string param;
	string junk;
	fs::ifstream file(filename);
	int fileLength;// = GetFile

	m_Joints.clear();
	m_Meshs.clear();

	file >> param;
	while(!file.eof())
	{

		if(param=="MD5Version")
		{
			file >> m_iMD5Version;
			assert(m_iMD5Version ==10);
		}else if (param == "commandline")
		{
			//IgnoreLine(file, fileLength ); // Ignore the contents of the line
		}
		else if (param == "numJoints")
		{
			file >> m_iNumJoint;
			m_Joints.reserve(m_iNumJoint);
		}
		else if (param == "numMeshes")
		{
			file >> m_iNumMesh;
			m_Meshs.reserve(m_iNumMesh);
		}
		else if (param == "joints")
		{
			Joint joint;
			file >>junk; // read "{" character
			for(int i=0; i<m_iNumJoint; i++)
			{
				file >> joint.m_name >> joint.m_ParentID >> junk
					>> joint.m_pos.x >> joint.m_pos.y >> joint.m_pos.z >> junk >> junk
					>> joint.m_Orient.x >> joint.m_Orient.y >> joint.m_Orient.z >> junk;
				//RemoveQuotes( joint.m_Name );
				//ComputeQuatW( joint.m_Orient );

				m_Joints.push_back(joint);
				//IgnoreLine( file, fileLength );
			}
			file >>junk; //read "}" character
		}else if (param == "mesh")
		{
			Mesh mesh;
			int numVerts, numTris, numWeights;
			
			file >> junk; // read {
			file >>param;
			while(param != "}")
			{
				if(param =="shader")
				{
					file >> mesh.m_Shader;
					//RemoveQuotes( mesh.m_Shader );
					fs::path shaderPath(mesh.m_Shader);
					fs::path texturePath;
					if(shaderPath.has_parent_path()) texturePath = shaderPath;
					else texturePath = parentPath / shaderPath;

					if(texturePath.has_extension()) texturePath.replace_extension(".tga");

					mesh.m_TexID = SOIL_load_OGL_texture(texturePath.string().c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS );
					file.ignore(fileLength, '\n');
				}else if (param=="numverts")
				{
					file >> numVerts;
					//IgnoreLine(file, fileLength);
					for(int i=0; i<numVerts; i++)
					{
						Vertex vert;
						file >>junk >> junk >> junk 
							>>  vert.m_Tex.x >> vert.m_Tex.y >> junk
							>> vert.m_startWeight >> vert.m_WeightCount;
						//IgnoreLine(file, fileLength);

						mesh.m_Verts.push_back(vert);
						mesh.m_TexBuffer.push_back(vert.m_Tex);
					}
				}else if (param == "numtris")
				{
					file >>numTris;
					//IgnoreLine(file, fileLength);
					for(int i=0; i<numTris; i++)
					{
						Triangle tri;
						file >>junk >>junk >> tri.m_Indices[0] >> tri.m_Indices[1] >> tri.m_Indices[2];

						//IgnoreLine(file, fileLength);

						mesh.m_Tris.push_back(tri);
						mesh.m_IndexBuffer.push_back(tri.m_Indices[0]);
						mesh.m_IndexBuffer.push_back(tri.m_Indices[1]);
						mesh.m_IndexBuffer.push_back(tri.m_Indices[2]);
					}
				}
				else if (param == "numweights")
				{
					file >> numWeights;
					//IgnoreLine(file, fileLength);
					for(int i = 0 ;i <numWeights; i++)
					{
						Weight weight;
						file >> junk >> junk >> weight.m_JointID >> weight.m_Bias >> junk
							>> weight.m_pos.x>> weight.m_pos.y>> weight.m_pos.z >> junk;
						//IgnoreLine(file, fileLength);
						mesh.m_Weights.push_back(weight);
					}
				}else 
				{
					//IgnoreLine(file, fileLength);
				}

				file >> param ;
			}

			PrepareMesh(mesh);
			PrepareNormal(mesh);
			m_Meshs.push_back(mesh);

		}

		file >>param;
	}

	assert(m_Joints.size() == m_iNumJoint);
	assert(m_Meshs.size() == m_iNumMesh);
	return true;
}


bool MD5Model::PrepareMesh(Mesh& mesh)
{
	mesh.m_PosBuffer.clear();
	mesh.m_TexBuffer.clear();

	for(int i=0; i<mesh.m_Verts.size(); i++)
	{
		Vec3 finalPos;
		Vertex& vert = mesh.m_Verts[i];
		vert.m_pos.zero();
		vert.m_normal.zero();

		for(int j = 0; j <vert.m_WeightCount; j++)
		{
			Weight& weight = mesh.m_Weights[vert.m_startWeight+j];
			Joint& joint = m_Joints[weight.m_JointID];

			Vec3 rotPos = joint.m_Orient * weight.m_pos;

			vert.m_pos +=(joint.m_pos + rotPos)*weight.m_Bias;
		}

		mesh.m_PosBuffer.push_back(vert.m_pos);
		mesh.m_TexBuffer.push_back(vert.m_Tex);
	}

	return true;
}

bool MD5Model::PrepareNormal(Mesh& mesh)
{
	mesh.m_NorBuffer.clear();

	for(int i=0; i<mesh.m_Tris.size(); i++)
	{
		Vec3 v0 = mesh.m_Verts[mesh.m_Tris[i].m_Indices[0]].m_pos;
		Vec3 v1 = mesh.m_Verts[mesh.m_Tris[i].m_Indices[1]].m_pos;
		Vec3 v2 = mesh.m_Verts[mesh.m_Tris[i].m_Indices[2]].m_pos;
		Vec3 normal = Math::Cross(v2-v0, v1 -v0);
		mesh.m_Verts[mesh.m_Tris[i].m_Indices[0]].m_normal+=normal;
		mesh.m_Verts[mesh.m_Tris[i].m_Indices[1]].m_normal+=normal;
		mesh.m_Verts[mesh.m_Tris[i].m_Indices[2]].m_normal+=normal;

	}

	for(int i=0;i<mesh.m_Verts.size(); i++)
	{
		Vertex& vert = mesh.m_Verts[i];
		Vec3 normal = Math::Normalize(vert.m_normal);
		mesh.m_NorBuffer.push_back(normal);
		vert.m_normal.zero();

		for(int j = 0; j<vert.m_WeightCount; j++)
		{
			const Weight& weight = mesh.m_Weights[vert.m_startWeight + j];
			const Joint& joint = m_Joints[weight.m_JointID];
			vert.m_normal +=(normal * joint.m_Orient) * weight.m_Bias;
		}

	}
	return true;
}


void MD5Model::Render()
{
	glPushMatrix();
	glMultMatrixf(m_LocalToWorldMatrix.ToFloatPtr());

	for(int i=0; i<m_Meshs.size(); i++)
		RenderMesh(m_Meshs[i]);

	//m_Animation.Render();
	for ( unsigned int i = 0; i < m_Meshs.size(); ++i )
    {
        RenderNormal( m_Meshs[i] );
    }
 
    glPopMatrix();
}


void MD5Model::RenderMesh(const Mesh& mesh)
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindTexture(GL_TEXTURE_2D,mesh.m_TexID);

	glVertexPointer(3, GL_FLOAT,0, &(mesh.m_PosBuffer[0]));
	glNormalPointer( GL_FLOAT, 0 ,&(mesh.m_NorBuffer[0]));
	glTexCoordPointer( 2, GL_FLOAT, 0, &(mesh.m_TexBuffer[0]));

	glDrawElements(GL_TRIANGLES, mesh.m_IndexBuffer.size(), GL_UNSIGNED_INT, &(mesh.m_IndexBuffer[0]));

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindTexture(GL_TEXTURE_2D,0);
}


void MD5Model::Update( float fDeltaTime )
{
    if ( m_bHasAnimation )
    {
        m_Animation.Update(fDeltaTime);
        const MD5Animation::FrameSkeleton& skeleton = m_Animation.GetSkeleton();
 
        for ( unsigned int i = 0; i < m_Meshs.size(); ++i )
        {
            PrepareMesh( m_Meshs[i], skeleton );
        }
    }
}

bool MD5Model::PrepareMesh( Mesh& mesh, const MD5Animation::FrameSkeleton& skel )
{
    for ( unsigned int i = 0; i < mesh.m_Verts.size(); ++i )
    {
        const Vertex& vert = mesh.m_Verts[i];
        Vec3& pos = mesh.m_PosBuffer[i];
		Vec3& normal = mesh.m_NorBuffer[i];
 
        pos.zero();
        normal.zero();
 
        for ( int j = 0; j < vert.m_WeightCount; ++j )
        {
            const Weight& weight = mesh.m_Weights[vert.m_startWeight + j];
            const MD5Animation::SkeletonJoint& joint = skel.m_Joints[weight.m_JointID];
 
            Vec3 rotPos = joint.m_Orient * weight.m_pos;
            pos += ( joint.m_Pos + rotPos ) * weight.m_Bias;
 
            normal += ( joint.m_Orient * vert.m_normal ) * weight.m_Bias;
        }
    }
    return true;
}
bool MD5Model::CheckAnimation( const MD5Animation& animation ) const
{
    if ( m_iNumJoint != animation.GetNumJoints() )
    {
        return false;
    }
 
    // Check to make sure the joints match up
    for ( unsigned int i = 0; i < m_Joints.size(); ++i )
    {
        const Joint& meshJoint = m_Joints[i];
        const MD5Animation::JointInfo& animJoint = animation.GetJointInfo( i );
 
        if ( meshJoint.m_name != animJoint.m_Name ||
             meshJoint.m_ParentID != animJoint.m_ParentID )
        {
            return false;
        }
    }
 
    return true;
}