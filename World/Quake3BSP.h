#pragma once
#include "Quake3BSPDef.h"
// This is our bitset class for storing which face has already been drawn.
// The bitset functionality isn't really taken advantage of in this version
// since we aren't rendering by leafs and nodes.
class CBitset
{

public:

	// Initialize all the data members
	CBitset() : m_bits(0), m_size(0) {}

	// This is our deconstructor
	~CBitset()
	{
		// If we have valid memory, get rid of it
		if (m_bits)
		{
			delete m_bits;
			m_bits = NULL;
		}
	}

	// This resizes our bitset to a size so each face has a bit associated with it
	void Resize(int count)
	{
		// Get the size of integers we need
		m_size = count / 32 + 1;

		// Make sure we haven't already allocated memory for the bits
		if (m_bits)
		{
			delete m_bits;
			m_bits = 0;
		}

		// Allocate the bits and initialize them
		m_bits = new unsigned int[m_size];
		ClearAll();
	}

	// This does the binary math to set the desired bit
	void Set(int i)
	{
		m_bits[i >> 5] |= (1 << (i & 31));
	}

	// This returns if the desired bit slot is a 1 or a 0
	int On(int i)
	{
		return m_bits[i >> 5] & (1 << (i & 31));
	}

	// This clears a bit to 0
	void Clear(int i)
	{
		m_bits[i >> 5] &= ~(1 << (i & 31));
	}

	// This initializes the bits to 0
	void ClearAll()
	{
		memset(m_bits, 0, sizeof(unsigned int) * m_size);
	}

private:

	// Our private bit data that holds the bits and size
	unsigned int *m_bits;
	int m_size;
};


// This is our Quake3 BSP class
class CQuake3BSP
{

public:

	// Our constructor
	CQuake3BSP();

	// Our deconstructor
	~CQuake3BSP();

	// This loads a .bsp file by it's file name (Returns true if successful)
	bool LoadBSP(const char *strFileName);

	// This renders the level to the screen, currently the camera pos isn't being used
	// THis function move to MapRender er
	// void RenderLevel(const Vec3 &vPos);

	// This destroys the level data
	void Destroy();

	int FindLeaf(const Vec3& pos);
	int IsClusterVisible(int current, int test);

	// This traces a single ray and checks collision with brushes
	Vec3 TraceRay(Vec3 vStart, Vec3 vEnd);

	// This traces a sphere along a ray to check for collision with the brushes
	Vec3 TraceSphere(Vec3 vStart, Vec3 vEnd, float radius);
	// This traces a axis-aligned bounding box (AABB) along a ray to check for collision
	Vec3 TraceBox(Vec3 vStart, Vec3 vEnd, Vec3 vMin, Vec3 vMax);

	// This tells us if we have just collided
	bool Collided() { return m_bCollided; }

private:

	// This attaches the correct extension to the file name, if found
	void FindTextureExtension(char *strFileName);
	Vec3 Trace(Vec3 vStart, Vec3 vEnd);

	// This recursively checks all the nodes until we find leafs that store the brushes
	void CheckNode(int nodeIndex, float startRatio, float endRatio, Vec3 vStart, Vec3 vEnd);

	// This checks our movement vector against the brush and it's sides
	void CheckBrush(tBSPBrush *pBrush, Vec3 vStart, Vec3 vEnd);
public:
	// This renders a single face to the screen
	// This function also move to MapRenderer
	// void RenderFace(int faceIndex);
	int m_numOfBrushes;			// The number of brushes in our world
	int m_numOfBrushSides;		// The number of brush sides in our world
	int m_numOfLeafBrushes;		// The number of leaf brushes

	int m_traceType;			// This stores if we are checking a ray, sphere or a box
	float m_traceRatio;			// This stores the ratio from our start pos to the intersection pt.
	float m_traceRadius;		// This stores the sphere's radius for a collision offset

	bool m_bCollided;			// This tells if we just collided or not
	int  m_numOfVerts;			// The number of verts in the model
	int  m_numOfFaces;			// The number of faces in the model
	int  m_numOfIndices;		// The number of indices for the model
	int  m_numOfTextures;		// The number of texture maps
	int  m_numOfLightmaps;		// The number of light maps

	int m_numOfNodes;			// The number of nodes in the level
	int m_numOfLeafs;			// The leaf count in the level
	int m_numOfLeafFaces;		// The number of leaf faces in the level
	int m_numOfPlanes;			// The number of planes in the level

	Vec3 m_vTraceMins;		// This stores the minimum values of the AABB (bottom corner)
	Vec3 m_vTraceMaxs;		// This stores the maximum values of the AABB (top corner)
	Vec3 m_vExtents;		// This stores the largest length of the box

	Vec3 m_vCollisionNormal;	// This stores the normal of the plane we collided with

	tBSPNode    *m_pNodes;		// The nodes in the bsp tree
	tBSPLeaf    *m_pLeafs;		// The leafs in the bsp tree
	tBSPPlane   *m_pPlanes;		// The planes stored in the bsp tree
	int         *m_pLeafFaces;	// The leaf's faces in the bsp tree
	tBSPVisData  m_clusters;	// The clusters in the bsp tree for space partitioning

	int			*m_pIndices;	// The object's indices for rendering
	tBSPVertex  *m_pVerts;		// The object's vertices
	tBSPFace	*m_pFaces;		// The faces information of the object
								// The texture and lightmap array for the level

	tBSPTexture   *m_pTextures;		// This stores our texture info for each brush
	tBSPBrush	  *m_pBrushes;		// This is our brushes
	tBSPBrushSide *m_pBrushSides;	// This holds the brush sides
	int			  *m_pLeafBrushes;  // The indices into the brush array

	Texture* m_textures[20];
	Texture* m_lightMap[20];
	CBitset m_FacesDrawn;		// The bitset for the faces that have/haven't been drawn
};

