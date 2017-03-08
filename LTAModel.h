#include <iostream>
#include <string>
#include <vector>

#include "Math\Vector.h"
#include "Math\Quaternion.h"
#include "Math\Matrix.h"

#include "LTAAnimation.h"
typedef Vec3 vec3;
typedef Quat quat;
typedef Mat4 mat4;
class LTAModel
{
public:
	LTAModel(){};
	virtual ~LTAModel(){};
	bool LoadModel( const std::string& filename );
    bool LoadAnim( const std::string& filename );
	
    void Update( float fDeltaTime );
	void Update(const LTAAnimation::JointList& skeleton);
    void Render();
	void DebugMesh(int id);
	void DebugWeight(int id);
protected:
	enum Return_Type
	{
		COMPILE,
		CHILDREN,
		TRANSFORM,
	};
	/*
	bool LoadHierarchy(const std::string& filename);
	Return_Type Load(FILE* pFile,int parentID);
	*/
	int FindID(std::string name);

	typedef std::vector<Vec3> PositionBuffer;
    typedef std::vector<Vec3> NormalBuffer;
    //typedef std::vector<Vec2> Tex2DBuffer;
    typedef std::vector<unsigned int> IndexBuffer;

	
	struct Vertex
    {
        vec3		m_Pos;
        vec3		m_Normal;
        int         m_StartWeight;
        int         m_WeightCount;
    };
    typedef std::vector<Vertex> VertexList;
 
    struct Triangle
    {
		int             m_Indices[3];
		Triangle(int a, int b, int c){m_Indices[0] = a; m_Indices[1] = b; m_Indices[2] = c;}
        
    };
    typedef std::vector<Triangle> TriangleList;
 
    struct Weight
    {
        int             m_JointID[2];
        float           m_Bias[2];
    };
	typedef std::vector<Weight> WeightList;

	struct Joint
    {
        std::string     m_Name;
        int             m_ParentID;
        vec3			m_Pos;
		quat			m_Orient;
		bool			m_flag;
    };
	typedef std::vector<Joint> JointList;
 
    struct Mesh
    {
		std::string		m_name;
        // This vertex list stores the vertices in the bind pose.
        VertexList      m_Verts;
        TriangleList    m_Tris;
        WeightList      m_Weights;
 
        // These buffers are used for rendering the animated mesh
        PositionBuffer  m_PositionBuffer;   // Vertex position stream
        NormalBuffer    m_NormalBuffer;     // Vertex normals stream
        //Tex2DBuffer     m_Tex2DBuffer;      // Texture coordinate set
        IndexBuffer     m_IndexBuffer;      // Vertex index buffer
    };
    typedef std::vector<Mesh> MeshList;
	// Prepare the mesh for rendering
    // Compute vertex positions and normals
    bool PrepareMesh( Mesh& mesh );
    //bool PrepareMesh( Mesh& mesh, const MD5Animation::FrameSkeleton& skel );
	bool PrepareNormals( Mesh& mesh );
 
    // Render a single mesh of the model
    void RenderMesh( const Mesh& mesh );
	void RenderNormals( const Mesh& mesh );
 
    // Draw the skeleton of the mesh for debugging purposes.
    void RenderSkeleton( const JointList& joints );
	
    //bool CheckAnimation( const MD5Animation& animation ) const;

private:
    int                 m_iNumJoints;
    int                 m_iNumMeshes;
 
    bool                m_bHasAnimation;
 
    JointList           m_Joints;
    MeshList            m_Meshes;
 
    //MD5Animation        m_Animation;
 
    mat4         m_LocalToWorldMatrix;
	std::vector<std::string> m_NodeList;
 
};


