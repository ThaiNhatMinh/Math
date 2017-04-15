#pragma once


#include "..\Math\Vector.h"
// This is the number that is associated with a face that is of type "polygon"
#define FACE_POLYGON	1
typedef unsigned char byte;

#define TYPE_RAY		0				// This is the type for tracing a RAY
#define TYPE_SPHERE		1				// This is the type for tracing a SPHERE
#define TYPE_BOX		2
const float kEpsilon = 0.03125f;		// This is our small number to compensate for float errors

// This is our integer vector structure
struct tVector3i
{
	int x, y, z;				// The x y and z position of our integer vector
};


// This is our BSP header structure
struct tBSPHeader
{
	char strID[4];				// This should always be 'IBSP'
	int version;				// This should be 0x2e for Quake 3 files
};


// This is our BSP lump structure
struct tBSPLump
{
	int offset;					// The offset into the file for the start of this lump
	int length;					// The length in bytes for this lump
};


// This is our BSP vertex structure
struct tBSPVertex
{
	Vec3 vPosition;			// (x, y, z) position. 
	Vec2 vTextureCoord;		// (u, v) texture coordinate
	Vec2 vLightmapCoord;	// (u, v) lightmap coordinate
	Vec3 vNormal;			// (x, y, z) normal vector
	byte color[4];				// RGBA color for the vertex 
};


// This is our BSP face structure
struct tBSPFace
{
	int textureID;				// The index into the texture array 
	int effect;					// The index for the effects (or -1 = n/a) 
	int type;					// 1=polygon, 2=patch, 3=mesh, 4=billboard 
	int startVertIndex;			// The starting index into this face's first vertex 
	int numOfVerts;				// The number of vertices for this face 
	int startIndex;				// The starting index into the indices array for this face
	int numOfIndices;			// The number of indices for this face
	int lightmapID;				// The texture index for the lightmap 
	int lMapCorner[2];			// The face's lightmap corner in the image 
	int lMapSize[2];			// The size of the lightmap section 
	Vec3 lMapPos;			// The 3D origin of lightmap. 
	Vec3 lMapVecs[2];		// The 3D space for s and t unit vectors. 
	Vec3 vNormal;			// The face normal. 
	int size[2];				// The bezier patch dimensions. 
};



// This is our BSP texture structure
struct tBSPTexture
{
	char strName[64];			// The name of the texture w/o the extension 
	int flags;					// The surface flags (unknown) 
								// This variable stores the types of brushes that we are dealing with.
								// Of the first binary bit is 1, then it's a solid object, otherwise
								// it's something like water or a sprite.
	int textureType;			// The type of texture (solid, water, slime, etc..) (type & 1) = 1 (solid)


};

// This is our BSP lightmap structure which stores the 128x128 RGB values
struct tBSPLightmap
{
	byte imageBits[128][128][3];   // The RGB data in a 128x128 image
};


// This stores a node in the BSP tree
struct tBSPNode
{
	int plane;					// The index into the planes array 
	int front;					// The child index for the front node 
	int back;					// The child index for the back node 
	tVector3i min;				// The bounding box min position. 
	tVector3i max;				// The bounding box max position. 
};

// This stores a leaf (end node) in the BSP tree
struct tBSPLeaf
{
	int cluster;				// The visibility cluster 
	int area;					// The area portal 
	tVector3i min;				// The bounding box min position 
	tVector3i max;				// The bounding box max position 
	int leafface;				// The first index into the face array 
	int numOfLeafFaces;			// The number of faces for this leaf 
	int leafBrush;				// The first index for into the brushes 
	int numOfLeafBrushes;		// The number of brushes for this leaf 
};

// This stores a splitter plane in the BSP tree
struct tBSPPlane
{
	Vec3 vNormal;			// Plane normal. 
	float d;					// The plane distance from origin 
};

// This stores the cluster data for the PVS's
struct tBSPVisData
{
	int numOfClusters;			// The number of clusters
	int bytesPerCluster;		// The amount of bytes (8 bits) in the cluster's bitset
	byte *pBitsets;				// The array of bytes that holds the cluster bitsets				
};



  // This stores the brush data
struct tBSPBrush
{
	int brushSide;				// The starting brush side for the brush 
	int numOfBrushSides;		// Number of brush sides for the brush
	int textureID;				// The texture index for the brush
};

// This stores the brush side data, which stores indices for the normal and texture ID
struct tBSPBrushSide
{
	int plane;					// The plane index
	int textureID;				// The texture index
};


// This is our lumps enumeration
enum eLumps
{
	kEntities = 0,				// Stores player/object positions, etc...
	kTextures,					// Stores texture information
	kPlanes,				    // Stores the splitting planes
	kNodes,						// Stores the BSP nodes
	kLeafs,						// Stores the leafs of the nodes
	kLeafFaces,					// Stores the leaf's indices into the faces
	kLeafBrushes,				// Stores the leaf's indices into the brushes
	kModels,					// Stores the info of world models
	kBrushes,					// Stores the brushes info (for collision)
	kBrushSides,				// Stores the brush surfaces info
	kVertices,					// Stores the level vertices
	kIndices,					// Stores the level indices
	kShaders,					// Stores the shader files (blending, anims..)
	kFaces,						// Stores the faces for the level
	kLightmaps,					// Stores the lightmaps for the level
	kLightVolumes,				// Stores extra world lighting information
	kVisData,					// Stores PVS and cluster info (visibility)
	kMaxLumps					// A constant to store the number of lumps
};


