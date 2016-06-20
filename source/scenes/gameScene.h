#pragma once

#include "../SceneManager/Scene.h"
#include "../game/ball.h"
#include "../game/targetTrail.h"
#include "../game/pathGenerator.h"
#include "../game/borderWall.h"

#define PATH_AVG_COUNT 5
#define BALL_SPEED	100.0f

class gameScene : public Scene
{
public:

	enum kGameState
	{
		State_Idle,
		State_EditPath,
		State_Simulate,
		State_EndSimulation,
	};

	gameScene();
	~gameScene();
	
	//scene callbacks
	void onInit();
	void onSize(int cx, int cy);
	void onUpdate(unsigned int dtm);
	void onRender();
	
	void onTouchBegin(int x, int y, void* touchPtr);
	void onTouchMoved(int x, int y, void* touchPtr);
	void onTouchEnd(int x, int y, bool bProcessed, void* touchPtr);
    
    bool loadResource(int userdefined);
    void onChildPoped(Scene* childscene, int msg){};

	void setGameState(kGameState state);
	void onGameStateChange();
	void followObject(float dt, objectBase* chasedObj);

	//collision
	void resetCollisionVars();

private:

	void doSimulate(float dt);

    CTextureManager m_cTextureManager;
	matrix4x4f m_cViewMatrix;

	ball m_cBall;
	targetTrail m_cTargetTrailEffect;
	borderWall m_cBorderWall;
	pathGenerator m_cPathGenerator;
	kGameState m_eGameState;
	bool m_bStopFollowCam;
	bool m_bCollisionOccuredOnWall;
	//float m_fElapsedTimeAfterFirstCollision;
};