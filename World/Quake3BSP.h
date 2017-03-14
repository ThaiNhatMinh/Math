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

public:

	// This attaches the correct extension to the file name, if found
	void FindTextureExtension(char *strFileName);

	// This renders a single face to the screen
	// This function also move to MapRenderer
	// void RenderFace(int faceIndex);

	int  m_numOfVerts;			// The number of verts in the model
	int  m_numOfFaces;			// The number of faces in the model
	int  m_numOfIndices;		// The number of indices for the model
	int  m_numOfTextures;		// The number of texture maps

	int			*m_pIndices;	// The object's indices for rendering
	tBSPVertex  *m_pVerts;		// The object's vertices
	tBSPFace	*m_pFaces;		// The faces information of the object
								// The texture and lightmap array for the level
	Texture* m_textures[20];

	CBitset m_FacesDrawn;		// The bitset for the faces that have/haven't been drawn
};

