#include "Scene.h"

Scene::Scene()
{
	m_pRoot = NULL;
	m_pMotion = NULL;
}

Scene::~Scene()
{
	delete m_pRoot;
	delete m_pMotion;
}

void Scene::Init()
{
	m_pMotion = new PlayerMotion;
	m_pMotion->Init();
	//m_Frustum.Init(45.0f, 4.0f / 3.0f, 1.0f, 2000.0f);
}

void Scene::Update(float dt)
{
	//m_pMotion->Update(dt);
	m_pCamera->Update(dt);
	m_pFrustum->Update(*m_pCamera);

	m_pRoot->OnUpdate(this,dt);
	
}

void Scene::RenderScene()
{
	// Render chilren first because sky is the last to draw for improve performance
	m_pRoot->RenderChildren(this);

	m_pRoot->PreRender(this);
	m_pRoot->Render(this);
	m_pRoot->PostRender(this);

}
