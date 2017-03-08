#include "Interface.h"
#include "Math\Vector.h"
#include "Math\Quaternion.h"
#include "Math\Matrix.h"
#include <iostream>
#include <vector>
#include <string>
typedef std::vector<ISceneNode*> SceneNodeList;

class SceneNode: public virtual ISceneNode
{
private:
	std::string m_sName;
	mat4		m_toWorld,m_fromWorld;
	float		m_fRadius;
	Vector3		m_vPosition;
	Quaternion	m_Orient;
	uint16_t	m_iID;

	static uint16_t	m_iCounterID;
protected:
	SceneNodeList	m_Children;
	ISceneNode		*m_pParent;
public:
	
	
	SceneNode(std::string name,const mat4* to);
	virtual~SceneNode();
	virtual void SetTransform(const mat4* toWorld, const mat4* fromWorld=NULL);
	virtual void OnUpdate( float deltaTime);
	virtual void OnRestore();
	virtual void PreRender() ;
	virtual bool IsVisible() ;
	virtual void Render() =0;
	virtual void RenderChildren() {};
	virtual void PostRender() ;

	virtual bool AddChild(ISceneNode* kid) ;
	virtual bool RemoveChild(int id);
	Vector3 GetPosition()const{return m_vPosition;};
	void SetPosition(const Vector3& pos){m_vPosition = pos;};
	void SetRadius(float radius){m_fRadius = radius;};

	
};