#pragma once

#include "..\iostream.h"
enum blendset
{
	upper,
	lower,
};

enum weaponanim
{
	idle,
	shoot,
	reload,
	c_walk,
	c_walkBside,
	c_walkRside,
	c_walkLside,
	walk,
	walkBside,
	walkRside,
	walkLside,
	run,
	runBside,
	runRside,
	runLside,


};
enum baseanim
{
	jump = 18,
	sniper = 66,

};
class CharPlayer;
struct MotionRegister
{
	float m_fTime;
	GLuint m_iCurrentFrame;
	GLuint m_iCurrentAnim;
	CharPlayer* m_This;

	/*	m_bOnAnimation value
		0 - ilde play animation	== no other animation so play idle
		1 - on playing animation
		2 - on blending animation

	*/
	GLuint m_bOnAnimation;
	
	// Time blend betwen 2 animation
	float m_fBlendTime;
	// FrameData for current frame. use for blending
	vector<FrameData> m_Frames;
};

class PlayerMotion
{
	friend class BaseAnim;
private:
	// base animation
	LTBProp				*m_pProp;
	vector<LTBMesh*>	m_pMeshs;
	vector<SkeNode*>	m_pNodeLists;
	vector<Animation*>	m_pAnimList;
	vector<string>		m_ChildModel;
	vector<LTBSocket>	m_Socket;

	vector<MotionRegister*> m_MotionRegister;

public:
	PlayerMotion() { };
	~PlayerMotion() ;

	void Init();
	//void Update(float dt);
	void SetData(BaseAnim* p);
};
