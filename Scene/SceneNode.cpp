#include "SceneNode.h"

uint16_t SceneNode::m_iCounterID = 0;

SceneNode::SceneNode(std::string name, const mat4 * to)
{
	m_iID = m_iCounterID;
	m_iCounterID++;
	m_sName = name;
	m_vPosition = Vec3(0, 0, 0);
	m_fRadius = 10;
}

inline SceneNode::~SceneNode()
{
	for (int i = 0; i < m_Children.size(); i++)
		delete m_Children[i];
}

void SceneNode::SetTransform(const mat4* toWorld, const mat4* fromWorld)
{
	m_toWorld = *toWorld;
	if(!fromWorld) m_fromWorld = m_toWorld.Inverse();
	else m_fromWorld = fromWorld;

}

void SceneNode::OnRestore()
{
	SceneNodeList::iterator i = m_Children.begin();
	SceneNodeList::iterator end = m_Children.end();
	while (i != end)
	{
		(*i)->OnRestore();
		++i;
	}
}
void SceneNode::OnUpdate(float deltaTime)
{
	SceneNodeList::iterator i = m_Children.begin();
	SceneNodeList::iterator end = m_Children.end();
	while (i != end)
	{
		(*i)->OnUpdate(deltaTime);
		++i;
	}
}

void SceneNode::PreRender()
{

}

bool SceneNode::IsVisible()
{
	return true;
}

void SceneNode::Render()
{
}

void SceneNode::PostRender()
{

}

bool SceneNode::AddChild(ISceneNode * kid)
{
	// Only add to m_Children vector
	m_Children.push_back(kid);
	// need set parent pointer to ...

	return true;
}

bool SceneNode::RemoveChild(int id)
{
	return false;
}
