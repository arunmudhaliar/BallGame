#pragma once

#include "../SceneManager/Scene.h"
#include "../engine/core/TextureManager.h"

#include "../SpriteManager/Sprite2Dx.h"
#include "ball.h"
#include "entityManager.h"
#include "targetTrail.h"

class gameScene : public Scene, public MEntityObserver
{
public:
	gameScene();
	~gameScene();
	
	//scene callbacks
	void onInit();
	void onSize(int cx, int cy);
    void onFixedUpdate(float dt);
	void onUpdate(unsigned int dtm);
	void onRender();
	void onExit();
	void onPause();
	void onResume();
	void onTimer(unsigned int msg);
	
	void onTouchBegin(int x, int y, void* touchPtr);
	void onTouchMoved(int x, int y, void* touchPtr);
	void onTouchEnd(int x, int y, bool bProcessed, void* touchPtr);
    
	//fader callbacks
	void onFaderComplete(unsigned int msg);
    
	//input callbacks
	void processKeyInputs();

	virtual void onKeyDown(int keyCode);
	virtual void onKeyUp(int keyCode);

    void onAnimationStart(unsigned int msg, float scale);   //called immediate after startAnimation()
    void onAnimation(unsigned int msg, float scale);
    void onAnimationEnd(unsigned int msg, float scale);

	//enityManager callbacks
	void onPreUpdateEntity(entityBase* entity);
	void onPostUpdateEntity(entityBase* entity);

    bool loadResource(int userdefined);
    void onChildPoped(Scene* childscene, int msg){};
    void onReloadOpneGLResources();

    CTextureManager m_cTextureManager;
    
    void* m_pTouchPtr;
    vector2f m_cPrevTouch;
	matrix4x4f m_cViewMatrix;

	ball m_cBall;
	targetTrail m_cTargetTrailEffect;
	Sprite2Dx m_cBorderWall;

	entityManager m_cEntityManager;
	bool m_bszKeyBuffers[256];

	float m_cRotation;
};