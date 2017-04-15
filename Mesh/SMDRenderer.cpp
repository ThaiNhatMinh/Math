#include "SMDRenderer.h"

void SMDRenderer::RenderMesh(SMDMesh * mesh)
{
	glBindVertexArray(mesh->m_iVAO);
	glDrawArrays(mesh->Topology, 0, mesh->m_vPositions.size());
}

SMDRenderer::SMDRenderer()
{
}

SMDRenderer::~SMDRenderer()
{
	delete m_pShader;
}

bool SMDRenderer::Init(const char * FileName)
{
	// init shader
	
	m_pShader = new Shader("Game\\Shader\\PlayerView.vs", "Game\\Shader\\PlayerView.frag");
	m_iModelUniform = m_pShader->GetUniformLocation("Model");
	m_iViewUniform = m_pShader->GetUniformLocation("View");
	m_iProjUniform = m_pShader->GetUniformLocation("Proj");
	m_iLightPosUniform = m_pShader->GetUniformLocation("lightPos");
	m_iEyePosUniform = m_pShader->GetUniformLocation("EyePos");
	m_iTI_ModelUniform = m_pShader->GetUniformLocation("TI_Model");
	m_pShader->LinkShader();
	m_SMD.Init(FileName);
	m_SMD.SetupMesh();
	
	for (size_t i = 0; i < m_SMD.m_BoneList.size(); i++)
	{
		char buffer[4];
		_itoa(i, buffer, 10);
		string s = buffer;
		m_iTransformUniform[i] = m_pShader->GetUniformLocation(("Transform[" + s + "]").c_str());
	}

	SMDAnimation a("draw");
	a.Load("Game\\Model\\SG552\\v_rif_sg552_anims\\empty_reload.smd", m_SMD.m_BoneList.size());
	m_AnimationList.push_back(a);
	return false;
}

void SMDRenderer::PreRender(mat4 & view, mat4 & proj,Camera& camera)
{
	m_pShader->Use();
	Quat ort;
	Vec2 rot = camera.GetRorate();
	
	float sp, sy, sr;
	float cp, cy, cr;
	Math::SinCos(sp, cp, Math::ToRadian(rot.x*0.5f));
	Math::SinCos(sy, cy, Math::ToRadian(rot.y*0.5f));
	Math::SinCos(sr, cr, Math::ToRadian(0*0.5f));

	ort.set(cr*cp*cy + sp*sy*sr, cr*sp*cy + sr*cp*sy, -cr*sp*sy + sy*cp*cy, -sr*sp*cy + cr*cp*sy);

	m_Model = ort.ToMatrix();

	mat4 TI_Model = m_Model.Inverse();
	TI_Model = TI_Model.Transpose();
	
	Vec3 pos = camera.GetPosition();
	pos = pos + Vec3(0.0,10.0,0.0);
	glUniformMatrix4fv(m_iModelUniform, 1, GL_FALSE, m_Model.ToFloatPtr());
	glUniformMatrix4fv(m_iViewUniform, 1, GL_FALSE, view.ToFloatPtr());
	glUniformMatrix4fv(m_iProjUniform, 1, GL_FALSE, proj.ToFloatPtr());
	glUniformMatrix4fv(m_iTI_ModelUniform, 1, GL_FALSE, TI_Model.ToFloatPtr());
	glUniform3f(m_iEyePosUniform,pos.x,pos.y,pos.z);
	glUniform3f(m_iLightPosUniform, 100, 100, 100);


	vector<Skeleton> ske;
	m_AnimationList[0].GetFrame(111, m_SMD.m_BoneList, ske);

	for (size_t i = 0; i < m_SMD.m_BoneList.size(); i++)
	{
		mat4 finalM = m_SMD.m_BoneList[i].Pose*ske[i].Transform ;
		glUniformMatrix4fv(m_iTransformUniform[i], 1, GL_FALSE, finalM.ToFloatPtr());
	}
	glClear(GL_DEPTH_BUFFER_BIT);

}

void SMDRenderer::Render()
{
	vector<SMDMesh*> meshlist = m_SMD.GetMesh();
	for (size_t i = 0; i < meshlist.size(); i++)
		RenderMesh(meshlist[i]);
}

void SMDRenderer::PostRender()
{
	//glEnable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	m_pShader->UnUse();
}
