#include "..\iostream.h"

void Frustum::Init()
{
	glGenVertexArrays(1, &m_iVAO);
	glGenBuffers(1, &m_iVBO);

	glBindVertexArray(m_iVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_iVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8 * 3, NULL, GL_DYNAMIC_DRAW);
	glGenBuffers(1, &m_iEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iEBO);
	int indices[] = { 0,1,1,2,2,3,3,0,4,5,5,6,6,7,7,4,0,4,3,7,1,5,2,6 };
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Frustum::Frustum()
{
	m_Fov = Math::PI/ 4.0f; // default is 90
	m_Aspect  = 4.0f / 3.0f;
	m_Near = 1.0f;
	m_Far = 1000.0f;


}

Frustum::~Frustum()
{
}

bool Frustum::Inside(const Vec3& v)const
{
	for (int i = 0; i < NumPlane; i++)
	{
		if (m_Plane[i].DistanceToPlane(v)<-5.0) return false;
	}
	return true;
}

bool Frustum::Inside(const Vec3& v, float radius)const
{
	for(int i=0; i<NumPlane; i++)
		if(!m_Plane[i].Inside(v,radius)) return false;

	return true;
}

bool Frustum::Inside(const Vec3 & v, const Vec3 & v2) const
{
	for (int i = 0; i < NumPlane; i++)
	{
		if (m_Plane[i].DistanceToPlane(Vec3(v.x,v.y,v.z))>0) continue;
		if (m_Plane[i].DistanceToPlane(Vec3(v2.x, v.y, v.z))>0) continue;
		if (m_Plane[i].DistanceToPlane(Vec3(v.x, v2.y, v.z))>0) continue;
		if (m_Plane[i].DistanceToPlane(Vec3(v2.x, v2.y, v.z))>0) continue;
		if (m_Plane[i].DistanceToPlane(Vec3(v.x, v.y, v2.z))>0) continue;
		if (m_Plane[i].DistanceToPlane(Vec3(v2.x, v.y, v2.z))>0) continue;
		if (m_Plane[i].DistanceToPlane(Vec3(v.x, v2.y, v2.z))>0) continue;
		if (m_Plane[i].DistanceToPlane(Vec3(v2.x, v2.y, v2.z))>0) continue;
		
		// 8 point outside of frustum then false test
		return false;
	}
	
	
	return true;
}

void Frustum::Update(const Camera & camera)
{

	
	Vec3 Forward = camera.m_Front;
	Vec3 Pos = camera.m_Position;
	Vec3 right = camera.m_Right;
	Vec3 Up = Math::Cross(Forward, right);

	float Hnear = 2 * tanFovOver2 * m_Near;
	float Wnear = Hnear * m_Aspect;
	float Hfar = 2 * tanFovOver2 * m_Far;
	float Wfar = Hfar * m_Aspect;

	Vec3 fc = Pos + Forward*m_Far;

	Vec3 ftl = fc + (Up*Hfar / 2) - (right * Wfar / 2);
	Vec3 ftr = fc + (Up*Hfar / 2) + (right * Wfar / 2);
	Vec3 fbl = fc - (Up*Hfar / 2) - (right * Wfar / 2);
	Vec3 fbr = fc - (Up*Hfar / 2) + (right * Wfar / 2);

	Vec3 nc = Pos + Forward*m_Near;

	Vec3 ntl = nc + (Up*Hnear / 2) - (right * Wnear / 2);
	Vec3 ntr = nc + (Up*Hnear / 2) + (right * Wnear / 2);
	Vec3 nbl = nc - (Up*Hnear / 2) - (right * Wnear / 2);
	Vec3 nbr = nc - (Up*Hnear / 2) + (right * Wnear / 2);
	/*
	Vec3 nearRight = right * ((m_Near * tanFovOver2) * m_Aspect);
	Vec3 farRight = right * ((m_Far * tanFovOver2) * m_Aspect);
	Vec3 nearUp = Up * (m_Near * tanFovOver2);
	Vec3 farUp = Up * (m_Far * tanFovOver2);

	m_NearClip[0] = camera.m_Position + (Forward  * m_Near) - nearRight + nearUp;
	m_NearClip[1] = camera.m_Position + (Forward  * m_Near) + nearRight + nearUp;
	m_NearClip[2] = camera.m_Position + (Forward  * m_Near) + nearRight - nearUp;
	m_NearClip[3] = camera.m_Position + (Forward  * m_Near) - nearRight - nearUp;

	m_FarClip[0] = camera.m_Position + (Forward  * m_Far) - farRight + farUp;
	m_FarClip[1] = camera.m_Position + (Forward  * m_Far) + farRight + farUp;
	m_FarClip[2] = camera.m_Position + (Forward  * m_Far) + farRight - farUp;
	m_FarClip[3] = camera.m_Position + (Forward  * m_Far) - farRight - farUp;

	m_Plane[Near].Init(m_NearClip[0], m_NearClip[1], m_NearClip[2]);
	m_Plane[Far].Init(m_FarClip[2], m_FarClip[1], m_FarClip[0]);
	m_Plane[Right].Init(m_FarClip[2], m_FarClip[1], Pos);
	m_Plane[Left].Init(m_FarClip[0], m_FarClip[3], Pos);
	m_Plane[Top].Init(m_FarClip[1], m_FarClip[0], Pos);
	m_Plane[Bottom].Init(m_FarClip[3], m_FarClip[2], Pos);
	*/

	m_NearClip[0] = ntl;
	m_NearClip[1] = ntr;
	m_NearClip[2] = nbr;
	m_NearClip[3] = nbl;

	m_FarClip[0] = ftl;
	m_FarClip[1] = ftr;
	m_FarClip[2] = fbr;
	m_FarClip[3] = fbl;

	m_Plane[Near].Init(ntl,ntr,nbr);
	m_Plane[Far].Init(fbr,ftr,ftl);
	m_Plane[Right].Init(ntr,ftr,fbr);
	m_Plane[Left].Init(ftl,ntl,nbl);
	m_Plane[Top].Init(ftl,ftr,ntr);
	m_Plane[Bottom].Init(nbr,fbr,fbl);

	
	GLfloat vertices[8][3] = {
		{ m_NearClip[0].x,m_NearClip[0].y,m_NearClip[0].z},
		{ m_NearClip[1].x,m_NearClip[1].y,m_NearClip[1].z },
		{ m_NearClip[2].x,m_NearClip[2].y,m_NearClip[2].z },
		{ m_NearClip[3].x,m_NearClip[3].y,m_NearClip[3].z },
		{ m_FarClip[0].x,m_FarClip[0].y,m_FarClip[0].z },
		{ m_FarClip[1].x,m_FarClip[1].y,m_FarClip[1].z },
		{ m_FarClip[2].x,m_FarClip[2].y,m_FarClip[2].z },
		{ m_FarClip[3].x,m_FarClip[3].y,m_FarClip[3].z },
	};
	int indices[] = { 0,1,2,0,2,3,4,5,6,4,6,7,0,4,7,0,7,3,1,5,6,1,6,2 };

	glBindBuffer(GL_ARRAY_BUFFER, m_iVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	
}

void Frustum::Render(Shader& shader,mat4& view, mat4& proj)
{
	shader.Use();
	mat4 model;
	//model.Translate(0, 0, 100);
	shader.SetUniformMatrix("Model", model.ToFloatPtr());
	shader.SetUniformMatrix("View", view.ToFloatPtr());
	shader.SetUniformMatrix("Proj", proj.ToFloatPtr());
	glBindVertexArray(m_iVAO);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);

}

mat4 Frustum::GetProjMatrix()
{
	return m_ProjMatrix;
}

void Frustum::Init(float fov, float aspect, float _Near, float _Far)
{
	m_Fov = Math::ToRadian(fov);
	m_Aspect = aspect;
	m_Near = _Near;
	m_Far = _Far;
	tanFovOver2 = Math::Tan(Math::ToRadian(fov/2));

	m_ProjMatrix = Math::Perspective(fov, aspect, _Near, _Far);
	
	
}