#pragma once
#include "..\iostream.h"


class Monster : public SkeEnity
{
protected:

protected:
	void LoadTexture(const char* FileName);

public:
	Monster() {};
	~Monster() {};
	bool Init(Scene*, const char* FileName);
	void SetAnimation(int);
	void AddAnimation(int);
	void NextAnim();

	void OnUpdate(Scene*, float);
	void PreRender(Scene*);
	void PostRender(Scene*);
};