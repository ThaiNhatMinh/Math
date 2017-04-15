
#include "..\iostream.h"

class Scene;

class ISceneNode
{
public:
	virtual void SetTransform(const mat4* toWorld, const mat4* fromWorld=NULL) = 0;
	virtual void OnUpdate(Scene*,float deltaTime) = 0;
	virtual void OnRestore(Scene*) = 0;
	virtual void PreRender(Scene*) = 0;
	virtual bool IsVisible(Scene*) = 0;
	virtual void Render(Scene*) = 0;
	virtual void RenderChildren(Scene*) = 0;
	virtual void PostRender(Scene*) = 0;

	virtual bool AddChild(ISceneNode* kid) = 0;
	virtual bool RemoveChild(int id) = 0;
	virtual int GetID() = 0;
	

	virtual ~ISceneNode(){};
};

