#include "..\iostream.h"

void LTBRenderer::RenderMesh(LTBMesh * mesh)
{
	mesh->m_pTexture->Bind(0);
	glBindVertexArray(mesh->m_iVAO);
	//glDrawArrays(mesh->Topology, 0, mesh->m_vPositions.size());
	glDrawElements(mesh->Topology, mesh->m_vIndices.size(), GL_UNSIGNED_INT, 0);
	mesh->m_pTexture->UnBind();
}

LTBRenderer::LTBRenderer()
{
}

LTBRenderer::~LTBRenderer()
{
	delete m_pShader;

}

bool LTBRenderer::Init(const char * FileName)
{
	// init shader

	m_pShader = new Shader("Game\\Shader\\Skeleton.vs", "Game\\Shader\\Skeleton.frag");
	m_pShader->LinkShader();
	m_iModelUniform = m_pShader->GetUniformLocation("Model");
	m_iViewUniform = m_pShader->GetUniformLocation("View");
	m_iProjUniform = m_pShader->GetUniformLocation("Proj");
	m_iLightPosUniform = m_pShader->GetUniformLocation("lightPos");
	m_iEyePosUniform = m_pShader->GetUniformLocation("EyePos");

	


	m_pLTB.Load(FileName);
	m_pLTB.SetupMesh(m_pShader);
	
	


	numNode = m_pLTB.m_nNodes;
	m_pMeshList = m_pLTB.m_pMeshs;


	string file = FileName;
	size_t t1 = file.find_last_of("\\/");
	size_t t2 = file.size() - t1;
	string texBodyFile = file.substr(t1, t2 - 4) + ".DTX";
	string texFaceFile = file.substr(t1, t2 - 11) + "FACE_BL.DTX";
	string TexPathFile = (file.substr(0, t1)) + texBodyFile;
	string TexFaceFile = (file.substr(0, t1)) + texFaceFile;
	m_pMeshList[0]->m_pTexture = Resources::LoadDTX(TexPathFile.c_str());
	m_pMeshList[1]->m_pTexture = Resources::LoadDTX(TexFaceFile.c_str());
	//m_pSkeleton = m_pLTB.m_pSkeleton;

	

	for (GLuint i = 0; i <numNode; i++)
	{
		char buffer[4];
		_itoa(i, buffer, 10);
		string s = buffer;
		m_iTransformUniform[i] = m_pShader->GetUniformLocation(("Transform[" + s + "]").c_str());
		m_iInvPoseUniform[i] = m_pShader->GetUniformLocation(("InvBindPose[" + s + "]").c_str());
	}
	
	
	return 1;
}

void LTBRenderer::SetAnimation(int a)
{
	m_pLTB.m_Animation.SetAnim(a);
}

void LTBRenderer::NextAnim()
{
	m_pLTB.m_Animation.NextAnim();
}

void LTBRenderer::SetTransform(mat4 transform)
{
	//cout << m_pSkeleton->m_Name << endl;
	//m_pSkeleton->m_GlobalTransform = transform;
	//m_pSkeleton->m_InvBindPose = transform.Inverse();
	m_Model = transform;
}

void LTBRenderer::Update(float dt)
{
	
	m_pLTB.m_Animation.Update(dt, jointlist);
	// CPU Skinning
	/*
	for (size_t i = 0; i < m_LTB.m_Meshs.size(); i++)
	{
		LTBMesh& mesh = m_LTB.m_Meshs[i];
		for (size_t j = 0; j < mesh.m_vVertex.size(); j++)
		{
			Vec3 pos = Vec3(0);
			Vertex& v = mesh.m_vVertex[j];
			for (size_t k = 0; k < 4; k++)
			{
				Weight& w = v.Weights[k];
				if (w.weight && w.Bone<60)
				{
					pos += jointlist[w.Bone].InvBindPose * jointlist[w.Bone].LocalTransform * mesh.m_vPositions[j] * w.weight;
				}
			}
			v.Pos = pos;
		}
		glBindVertexArray(mesh.m_iVAO);
		glBindBuffer(GL_ARRAY_BUFFER, mesh.m_iVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, mesh.m_vVertex.size()*sizeof(Vertex), &mesh.m_vVertex[0]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}*/


}

void LTBRenderer::PreRender(mat4 & view, mat4 & proj, Camera& camera)
{
	m_pShader->Use();
	
	Vec3 pos = camera.GetPosition();
	//pos = pos + Vec3(0.0, 10.0, 0.0);
	glUniformMatrix4fv(m_iModelUniform, 1, GL_FALSE, m_Model.ToFloatPtr());
	glUniformMatrix4fv(m_iViewUniform, 1, GL_FALSE, view.ToFloatPtr());
	glUniformMatrix4fv(m_iProjUniform, 1, GL_FALSE, proj.ToFloatPtr());
	glUniform3f(m_iEyePosUniform, pos.x, pos.y, pos.z);
	glUniform3f(m_iLightPosUniform, -100, -100, -100);
	
	if (jointlist.size())
	{
		for (GLuint i = 0; i < numNode; i++)
		{
			mat4 tt = m_pLTB.m_pNodeLists[i]->m_InvBindPose * jointlist[i].LocalTransform;

			//m_pShader->SetUniformMatrix(m_iInvPoseUniform[i], jointlist[i].InvBindPose.ToFloatPtr());
			m_pShader->SetUniformMatrix(m_iTransformUniform[i], tt.ToFloatPtr());
		}
	}
	else
	{
		for (GLuint i = 0; i < numNode; i++)
		{
			mat4 tt;// = m_pLTB.m_pNodeLists[i]->m_InvBindPose * jointlist[i].LocalTransform;

			//m_pShader->SetUniformMatrix(m_iInvPoseUniform[i], jointlist[i].InvBindPose.ToFloatPtr());
			m_pShader->SetUniformMatrix(m_iTransformUniform[i], tt.ToFloatPtr());
		}
	}

	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void LTBRenderer::Render()
{
	for (size_t i = 0; i < m_pMeshList.size(); i++)
		RenderMesh(m_pMeshList[i]);
}

void LTBRenderer::PostRender()
{
	//glEnable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	m_pShader->UnUse();
	glFrontFace(GL_CW);
	glDisable(GL_BLEND);
}
