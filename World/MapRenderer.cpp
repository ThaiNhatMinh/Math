#include "..\iostream.h"

void MapRenderer::RenderFace(int i)
{
	m_pFace[i].m_pTexture->Bind(0);
	//m_pShader->SetUniform("texMap", 0);
	glUniform1i(m_iTexUniform, 0);
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
		m_pFace[i].SetTexture(m_pBSP->m_textures);
	}
	m_pShader = new Shader();
	m_pShader->Load("Game\\Shader\\basic_shader.vert", "Game\\Shader\\basic_shader.frag");



	m_iModelUniform = m_pShader->GetUniformLocation("Model");
	m_iViewUniform = m_pShader->GetUniformLocation("View");
	m_iProjUniform = m_pShader->GetUniformLocation("Proj");
	m_iTexUniform = m_pShader->GetUniformLocation("texMap");
	m_Model.Translate(0, 0, 0);
	m_FacesDrawn.Resize(m_pBSP->m_numOfFaces);

}

void MapRenderer::PreRender(mat4& view, mat4& proj)
{
	m_pShader->Use();
	glUniformMatrix4fv(m_iModelUniform, 1, GL_FALSE, m_Model.ToFloatPtr());
	glUniformMatrix4fv(m_iViewUniform, 1, GL_FALSE, view.ToFloatPtr());
	glUniformMatrix4fv(m_iProjUniform, 1, GL_FALSE, proj.ToFloatPtr());
}

void MapRenderer::Render()
{
	// Get the number of faces in our level
	int i = m_pBSP->m_numOfFaces;

	// Reset our bitset so all the slots are zero.  This isn't really
	// utilized in this tutorial, but I thought I might as well add it
	// not so that we spread out the code between the tutorials.
	m_FacesDrawn.ClearAll();
	
	// Go through all the faces
	while (i--)
	{
		// Before drawing this face, make sure it's a normal polygon
		if (m_pFace[i].m_pFaces->type != FACE_POLYGON) continue;

		// If this face already hasn't been drawn
		if (!m_FacesDrawn.On(i))
		{
			// Set this face as drawn and render it
			m_FacesDrawn.Set(i);
			RenderFace(i);
		}
	}
}

void MapRenderer::PostRender()
{
	m_pShader->UnUse();
}
