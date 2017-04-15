#pragma once

#include "Quake3BSPDef.h"
#include "..\iostream.h"

class BSPMesh : public Mesh
{
public:
	tBSPFace	*m_pFaces;
	Texture* m_pLightMap;
	vector<Vec2> m_vlightUV;
public:
	BSPMesh();
	~BSPMesh();
	void Init(tBSPFace *pFaces, tBSPVertex  *pVerts, int*pIndices);
	void SetTexture(Texture* pTex[],Texture* pLightMap[]);
	void Finalize();
};