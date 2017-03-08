#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Math\Vector.h"
#include "Math\Quaternion.h"
#include "Math\Matrix.h"

#include "MD5Animation.h"
using std::vector;
using std::string;
class MD5Model
{
public:
	MD5Model();
	virtual ~MD5Model();

	bool LoadModel(const string& filename);
	bool LoadAnim(const string& filename);
	void Update(float deltaTime);
	void Render();
protected:
	typedef vector<Vector3> PositionBuffer;
	typedef vector<Vector3> NormalBuffer;
	typedef vector<Vector2> TexBuffer;
	typedef vector<int> IndexBuffer;

	struct Vertex
	{
		Vector3		m_pos;
		Vector3		m_normal;
		Vector2		m_Tex;
		int			m_startWeight;
		int			m_WeightCount;
	};

	typedef vector<Vertex> VertexList;

	struct Triangle
	{
		int			m_Indices[3];
	};

	typedef vector<Triangle> TriangleList;

	struct Weight
	{
		int			m_JointID;
		float		m_Bias;
		Vector3		m_pos;
	};

	typedef vector<Weight> WeightList;

	struct Joint
	{
		string		m_name;
		int			m_ParentID;
		Vector3		m_pos;
		Quat		m_Orient;
	};
	typedef vector<Joint> JointList;

	struct Mesh
	{
		string		m_Shader;
		VertexList	m_Verts;
		TriangleList m_Tris;
		WeightList	m_Weights;

		int			m_TexID;
		PositionBuffer	m_PosBuffer;
		NormalBuffer	m_NorBuffer;
		TexBuffer		m_TexBuffer;
		IndexBuffer		m_IndexBuffer;
	};
	typedef vector<Mesh> MeshList;

	// prepare meshs for rendering
	//computer pos and normal

	bool PrepareMesh(Mesh& mesh);
	bool PrepareMesh(Mesh& mesh,const MD5Animation::FrameSkeleton& skel );
	//bool PrepareMesh(Mehs& mesh, const MD5Animation::FrameSkeleton& skel );
	bool PrepareNormal(Mesh& mesh);
	void RenderMesh(const Mesh& mesh);
	void RenderNormal(const Mesh& mesh);
	bool CheckAnimation( const MD5Animation& animation ) const;
	// Draw the skeleton of the mesh for debugging purposes.
	void RenderSkeleton(const JointList& joints);

	// bool CheckAnimation(const MD5Animation& animation ) const;

private:
	int				m_iMD5Version;
	int				m_iNumJoint;
	int				m_iNumMesh;
	bool			m_bHasAnimation;
	JointList		m_Joints;
	MeshList		m_Meshs;
	MD5Animation    m_Animation;
	Mat4			m_LocalToWorldMatrix;
};

