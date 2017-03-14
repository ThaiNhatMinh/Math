#pragma once

#include "Quake3BSPDef.h"
#include "..\iostream.h"

class BSPMesh : public Mesh
{
public:
	tBSPFace	*m_pFaces;
public:
	BSPMesh();
	~BSPMesh();
	void Init(tBSPFace *pFaces, tBSPVertex  *pVerts, int*pIndices);
	void SetTexture(Texture* pTex[]);

};