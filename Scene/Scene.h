#pragma once
#include "..\iostream.h"


class PlayerMotion;

class Scene
{
private:
	Camera* m_pCamera;
	Frustum* m_pFrustum;
	SceneNode* m_pRoot; // it must be a sky box
	
	vector<PointLight> m_PointLights;
	vector<Spotlight> m_SpotLights;
	
	PlayerMotion* m_pMotion;
public:

	Scene();
	~Scene();
	void Init();
	void Update(float dt);
	void RenderScene();
	
	const Camera& GetCamera()const { return *m_pCamera; }
	const Frustum& GetFrustum()const { return *m_pFrustum; }
	void SetCamera(Camera* cam) { m_pCamera = cam; }
	void SetFrustum(Frustum* f) { m_pFrustum = f; }
	void SetRootScene(SceneNode* pRoot) { m_pRoot = pRoot; }

	void AddPointLight(const PointLight& pl) { m_PointLights.push_back(pl); }
	vector<PointLight>& GetPointLights(){ return m_PointLights; }
	void AddSpotLight(const Spotlight& sl) { m_SpotLights.push_back(sl); }
	vector<Spotlight>& GetSpotLights() { return m_SpotLights; }

	PlayerMotion* GetMotion() { return m_pMotion; };
};