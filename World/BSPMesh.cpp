#include "..\iostream.h"




void BSPMesh::Init(tBSPFace *pFaces, tBSPVertex  *pVerts, int*pIndices)
{
	for (int i = 0; i < pFaces->numOfVerts; i++)
	{
		m_vPositions.push_back(pVerts[pFaces->startVertIndex + i].vPosition);
		m_vNormals.push_back(pVerts[pFaces->startVertIndex + i].vNormal);
		Vec2 uv = pVerts[pFaces->startVertIndex + i].vTextureCoord;
		//while (uv.x > 1.0f) uv.x -= 1.0f;
		//while (uv.y > 1.0f) uv.y -= 1.0f;
		m_vUVs.push_back(uv);
		//cout << m_vUVs[i].x << ' ' << m_vUVs[i].y << endl;
	}

	for (int i = 0; i < pFaces->numOfIndices; i++)
	{
		m_vIndices.push_back(pIndices[pFaces->startIndex + i]);
	}


	m_pFaces = pFaces;
	Topology = GL_TRIANGLES;

	Finalize();
}

BSPMesh::BSPMesh()
{
}

BSPMesh::~BSPMesh()
{
}

void BSPMesh::SetTexture(Texture * pTex[])
{
	m_pTexture = pTex[m_pFaces->textureID];
}
