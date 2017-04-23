#include "..\iostream.h"
typedef std::vector<ISceneNode*> SceneNodeList;

class SceneNode: public virtual ISceneNode
{
private:
	std::string m_sName;
	uint16_t	m_iID;

	static uint16_t	m_iCounterID;
protected:
	SceneNodeList	m_Children;
	ISceneNode		*m_pParent;
	Vector3			m_vPosition;
	Quaternion		m_Orient;
	mat4			m_Model;
	mat4			m_InvModel;
public:
	
	
	SceneNode(std::string name);
	virtual~SceneNode();
	virtual void SetTransform(const mat4* toWorld, const mat4* fromWorld=NULL);
	// This must be call last in OnUpdate class to update children
	virtual void OnUpdate(Scene*, float deltaTime);
	virtual void OnRestore(Scene*);
	//virtual void PreRender(Scene*) ;
	virtual bool IsVisible(Scene*) { return true; };
	//virtual void Render(Scene*) =0;
	virtual void RenderChildren(Scene*);
	//virtual void PostRender(Scene*) ;

	virtual int GetID() { return m_iID; };
	virtual bool AddChild(ISceneNode* kid) ;
	virtual bool RemoveChild(int id);
	Vector3 GetPosition()const{return m_vPosition;};
	void SetPosition(const Vector3& pos) { m_vPosition = pos;};
	string GetName() { return m_sName; }
	
};