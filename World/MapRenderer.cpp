#include "..\iostream.h"
#include "MapRenderer.h"

void MapRenderer::RenderFace(int i)
{
	m_pFace[i].m_pTexture->Bind(0);
	glUniform1i(m_iTexUniform, 0);
	m_pFace[i].m_pLightMap->Bind(1);
	glUniform1i(m_iLightMapUniform, 1);
	glBindVertexArray(m_pFace[i].m_iVAO);
	glDrawElements(m_pFace[i].Topology, m_pFace[i].m_vIndices.size(), GL_UNSIGNED_INT, 0);
}

MapRenderer::MapRenderer()
{
	CQuake3BSP* m_pBSP = NULL;
	Shader* m_pShader = NULL;
}

MapRenderer::~MapRenderer()
{
	delete [] m_pFace;
	delete m_pBSP;
}

void MapRenderer::Init(const char * mapFile)
{

	m_pBSP = new CQuake3BSP();
	m_pBSP->LoadBSP(mapFile);
	m_pFace = new BSPMesh[m_pBSP->m_numOfFaces];
	for (int i = 0; i < m_pBSP->m_numOfFaces; i++)
	{
		m_pFace[i].Init(&m_pBSP->m_pFaces[i], m_pBSP->m_pVerts, m_pBSP->m_pIndices);
		m_pFace[i].SetTexture(m_pBSP->m_textures,m_pBSP->m_lightMap);
	}
	m_pShader = new Shader();
	m_pShader->Load("Game\\Shader\\basic_shader.vert", "Game\\Shader\\basic_shader.frag");



	m_iModelUniform = m_pShader->GetUniformLocation("Model");
	m_iViewUniform = m_pShader->GetUniformLocation("View");
	m_iProjUniform = m_pShader->GetUniformLocation("Proj");
	m_iTexUniform = m_pShader->GetUniformLocation("texMap");
	m_iLightMapUniform = m_pShader->GetUniformLocation("lightMap");
	m_Model.Translate(0, 0, 0);
	m_FacesDrawn.Resize(m_pBSP->m_numOfFaces);

}

void MapRenderer::Update(Camera& camera, float dt)
{
	Vec3 OldPos = camera.m_Position;
	camera.Update(dt);
	Vec3 CurrentPos = camera.m_Position;
	camera.m_Position = m_pBSP->TraceBox(OldPos, CurrentPos, Vec3(-20, -50, -20), Vec3(20, 50, 20));

}

void MapRenderer::PreRender(mat4& view, mat4& proj)
{
	m_pShader->Use();
	glUniformMatrix4fv(m_iModelUniform, 1, GL_FALSE, m_Model.ToFloatPtr());
	glUniformMatrix4fv(m_iViewUniform, 1, GL_FALSE, view.ToFloatPtr());
	glUniformMatrix4fv(m_iProjUniform, 1, GL_FALSE, proj.ToFloatPtr());
}

void MapRenderer::Render(const Frustum& f, const Vec3& pos)
{
	
	m_FacesDrawn.ClearAll();
	
	// Grab the leaf index that our camera is in
	int leafIndex = m_pBSP->FindLeaf(pos);

	// Grab the cluster that is assigned to the leaf
	int cluster = m_pBSP->m_pLeafs[leafIndex].cluster;

	// Initialize our counter variables (start at the last leaf and work down)
	int i = m_pBSP->m_numOfLeafs;
	// Go through all the faces
	while (i--)
	{
		// Get the current leaf that is to be tested for visibility from our camera's leaf
		tBSPLeaf *pLeaf = &(m_pBSP->m_pLeafs[i]);

		// If the current leaf can't be seen from our cluster, go to the next leaf
		if (!m_pBSP->IsClusterVisible(cluster, pLeaf->cluster))	continue;
		Vec3 min = Vec3((float)pLeaf->min.x, (float)pLeaf->min.y, (float)pLeaf->min.z);
		Vec3 max = Vec3((float)pLeaf->max.x, (float)pLeaf->max.y, (float)pLeaf->max.z);
		if (!f.Inside(min, max)) continue;

		// If we get here, the leaf we are testing must be visible in our camera's view.
		// Get the number of faces that this leaf is in charge of.
		int faceCount = pLeaf->numOfLeafFaces;

		// Loop through and render all of the faces in this leaf
		while (faceCount--)
		{
			// Grab the current face index from our leaf faces array
			int faceIndex = m_pBSP->m_pLeafFaces[pLeaf->leafface + faceCount];

			// Before drawing this face, make sure it's a normal polygon
			if (m_pFace[faceIndex].m_pFaces->type != FACE_POLYGON) continue;

			// Since many faces are duplicated in other leafs, we need to
			// make sure this face already hasn't been drawn.
			if (!m_FacesDrawn.On(faceIndex))
			{
				// Set this face as drawn and render it
				m_FacesDrawn.Set(faceIndex);
				RenderFace(faceIndex);
			}
		}
	}
}

void MapRenderer::PostRender()
{
	m_pShader->UnUse();
}
