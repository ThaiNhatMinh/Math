/* 
 * File:   BspRenderer.cpp
 * Author: dude719
 * 
 * Created on May 1, 2015, 1:08 AM
 */

#include "BspRenderer.h"


BspRenderer::BspRenderer(BSP* bsp, Camera* cam) 
	: m_bsp(bsp),
	m_camera(cam),
	m_bWireframe(false),
	m_bBrushEntityNames(false),
	bInit(false)
{
	Init();
}

BspRenderer::BspRenderer(const BspRenderer& orig)
{
}

BspRenderer::~BspRenderer()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
	delete m_bspShader;
}

void BspRenderer::Init()
{
	if (bInit)
		return;
	
	/* Initialize basic shader */
	m_bspShader = new Shader("Game\\Shader\\basic_shader.vert","Game\\Shader\\basic_shader.frag") ;
	
	
	m_bspShader->Use();
	
    m_MVPUniform = m_bspShader->GetUniformLocation("MVP"); 
    m_modelUniform = m_bspShader->GetUniformLocation("model");
	m_viewUniform = m_bspShader->GetUniformLocation("view");
	m_projectionUniform = m_bspShader->GetUniformLocation("projection");
    
    m_lightUniform = m_bspShader->GetUniformLocation("lightPos");
    
	//m_useUserColorUniform = m_bspShader->GetUniformLocation("useUserColor");
	//m_userColorUniform = m_bspShader->GetUniformLocation("userColor");
	
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_bsp->vertices.size() * sizeof(Vec3), m_bsp->vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &m_normalVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_normalVBO);
	glBufferData(GL_ARRAY_BUFFER, m_bsp->normals.size() * sizeof(Vec3), m_bsp->normals.data(), GL_STATIC_DRAW);
		
	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_bsp->indices.size() * sizeof(GLuint), m_bsp->indices.data(), GL_STATIC_DRAW);
	
	m_positionAttribute = m_bspShader->GetAttribLocation("position");
	glEnableVertexAttribArray(m_positionAttribute);
	glVertexAttribPointer(m_positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

	m_normalPositionAttribute = m_bspShader->GetAttribLocation("normal");
	glEnableVertexAttribArray(m_normalPositionAttribute);
	glVertexAttribPointer(m_normalPositionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
    m_bspShader->Use();
	
	bInit = true;
    
}

void BspRenderer::Begin()
{
	glBindVertexArray(m_vao);
	
    glEnableVertexAttribArray(m_positionAttribute);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);  
	glVertexAttribPointer(m_positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    glEnableVertexAttribArray(m_normalPositionAttribute);
    glBindBuffer(GL_ARRAY_BUFFER, m_normalVBO);
	glVertexAttribPointer(m_normalPositionAttribute, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
	m_bspShader->Use();
}

void BspRenderer::Render(mat4& view, mat4& proj)
{
	Quat Ort;
	Ort.setRotateXAxis(90.0f);
	mat4 Model = Ort.ToMatrix();
	Model.Translate(0, 0, 0);
	mat4 MVP = proj * view * Model;
	glUniformMatrix4fv(m_MVPUniform, 1, GL_FALSE, MVP.ToFloatPtr());
	glUniformMatrix4fv(m_modelUniform, 1, GL_FALSE, Model.ToFloatPtr());
	glUniformMatrix4fv(m_viewUniform, 1, GL_FALSE, view.ToFloatPtr());
	glUniformMatrix4fv(m_projectionUniform, 1, GL_FALSE, proj.ToFloatPtr());
    
    glUniform3f(m_lightUniform, m_camera->GetPosition().x, m_camera->GetPosition().y, m_camera->GetPosition().z);
    
	//glUniform1i(m_useUserColorUniform, GL_TRUE);
    
	//glUniform4f(m_userColorUniform, 0.9f, 0.9f, 0.9f, 1.0f);
		
	if (!m_bWireframe)
		glDrawElements(GL_TRIANGLES, m_bsp->worldSize, GL_UNSIGNED_INT, 0);
    //else
    
	glUniform4f(m_userColorUniform, 0.0f, 0.0f, 1.0f, 1.0f);
    
    glDrawElements(GL_LINE_LOOP, m_bsp->worldSize, GL_UNSIGNED_INT, 0);
	
	glUniform1i(m_useUserColorUniform, GL_FALSE);
    
    glEnable(GL_LINE_SMOOTH);
    

	m_bsp->RenderBrushEntities(m_modelUniform, m_userColorUniform, m_useUserColorUniform);
	//m_bsp->RenderProps(m_modelUniform, m_userColorUniform); 

	float white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	if (m_bBrushEntityNames)
		m_bsp->RenderBrushEntityNames(m_camera, white);
}

void BspRenderer::End()
{
	glDisableVertexAttribArray(m_positionAttribute);
    glDisableVertexAttribArray(m_normalPositionAttribute);
    
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_NORMALIZE);
}

void BspRenderer::ToggleWireframe()
{
	m_bWireframe = !m_bWireframe;
}

void BspRenderer::ToggleBrushEntityNames()
{
	m_bBrushEntityNames = !m_bBrushEntityNames;
}
