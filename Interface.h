#include "Math\Matrix.h"



class ISceneNode
{
public:
	virtual void SetTransform(const mat4* toWorld, const mat4* fromWorld=NULL) = 0;
	virtual void OnUpdate(float deltaTime) = 0;
	virtual void OnRestore() = 0;
	virtual void PreRender() = 0;
	virtual bool IsVisible() = 0;
	virtual void Render() = 0;
	virtual void RenderChildren() = 0;
	virtual void PostRender() = 0;

	virtual bool AddChild(ISceneNode* kid) = 0;
	virtual bool RemoveChild(int id) = 0;

	

	virtual ~ISceneNode(){};
};

