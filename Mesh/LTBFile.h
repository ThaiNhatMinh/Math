#pragma once
#include "..\iostream.h"
#include "LTBDef.h"

class LTBFile
{
public:
	uint32		m_nKeyFrames;		// Number of keyframes.
	uint32		m_nParentAnims;		// Number of animations (that come from us).
	uint32		m_nNodes;			// Number of nodes.
	uint32		m_nPieces;			// Number of pieces.
	uint32		m_nChildModels;		// Number of child models (including the self child model).
	uint32		m_nTris;			// Number of triangles.
	uint32		m_nVerts;			// Number of vertices.
	uint32		m_nVertexWeights;	// Number of vertex weights.
	uint32		m_nLODs;			// Number of LODs.
	uint32		m_nSockets;			// Number of sockets.
	uint32		m_nWeightSets;		// Number of weight sets.
	uint32		m_nAnimData;		// Amount of animation data
	uint32		m_nStrings;			// How many strings we're allocating.
	uint32		m_StringLengths;	// Sum of all string lengths (not including null terminator).
	uint32		m_VertAnimDataSize; // byte size of the vertex animation data.
									//file positions for fields that need to be updated after a certain phase has passed
	uint32		m_nAnimDataPos;		// the position of the animation data size in the file
	float		m_GlobalRadius;		// Global radius.. MUST enclose the whole model.
	uint32		m_iNumEnabledOBBs;	// Number of enable OBB
	

	vector<LTBMesh*> m_pMeshs;
	//SkeletonNode* m_pSkeleton;
	vector<SkeNode*> m_pNodeLists;
	SkeAnim		m_Animation;
protected:
	bool LoadSkeleton(FILE* pFile, SkeNode* p);
public:
	LTBFile();
	LTBFile(const char* filename);
	~LTBFile();
	bool Load(const char* filename);
	void SetupMesh(Shader*);
};