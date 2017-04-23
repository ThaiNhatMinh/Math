#include "..\iostream.h"

uint16_t SceneNode::m_iCounterID = 0;

SceneNode::SceneNode(std::string name)
{
	m_iID = m_iCounterID;
	m_iCounterID++;
	m_sName = name;
	m_vPosition = Vec3(0, 0, 0);
	m_Orient.setRotateYAxis(0);
}

inline SceneNode::~SceneNode()
{
	for (size_t i = 0; i < m_Children.size(); i++)
		delete m_Children[i];
}

void SceneNode::SetTransform(const mat4* toWorld, const mat4* fromWorld)
{
	m_Model = *toWorld;
	if(fromWorld==NULL) m_InvModel = m_Model.Inverse();
	else m_InvModel = fromWorld;

}

void SceneNode::OnRestore(Scene* pScene)
{
	SceneNodeList::iterator i = m_Children.begin();
	SceneNodeList::iterator end = m_Children.end();
	while (i != end)
	{
		(*i)->OnRestore(pScene);
		++i;
	}
}
void SceneNode::RenderChildren(Scene * pScene)
{
	// Iterate through the children....
	SceneNodeList::iterator i = m_Children.begin();
	SceneNodeList::iterator end = m_Children.end();

	for (size_t i = 0; i < m_Children.size();i++)
	{
		m_Children[i]->PreRender(pScene);
		m_Children[i]->Render(pScene);
		m_Children[i]->PostRender(pScene);
		m_Children[i]->RenderChildren(pScene);
	}
}
void SceneNode::OnUpdate(Scene* pScene,float deltaTime)
{
	SceneNodeList::iterator i = m_Children.begin();
	SceneNodeList::iterator end = m_Children.end();
	while (i != end)
	{
		(*i)->OnUpdate(pScene,deltaTime);
		++i;
	}
}
/*
void SceneNode::PreRender(Scene* pScene)
{

}

bool SceneNode::IsVisible(Scene* pScene)
{
	return true;
}

void SceneNode::Render(Scene* pScene)
{
}

void SceneNode::PostRender(Scene* pScene)
{

}
*/
bool SceneNode::AddChild(ISceneNode * kid)
{
	// Only add to m_Children vector
	m_Children.push_back(kid);
	// need set parent pointer to ...

	return true;
}

bool SceneNode::RemoveChild(int id)
{
	for (size_t i = 0; i < m_Children.size(); i++)
		if (m_Children[i]->GetID() == id)
		{
			m_Children.erase(m_Children.begin() + i);
			return true;
		}

	return false;
}
