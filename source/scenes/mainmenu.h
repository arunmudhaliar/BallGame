#ifndef MAINMENU_H
#define MAINMENU_H

#include "../SceneManager/Scene.h"

class mainMenu : public Scene
{
public:
	mainMenu();
	~mainMenu();
	
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


    bool loadResource(int userdefined);
    void onChildPoped(Scene* childscene, int msg){};
    void onReloadOpneGLResources();

	matrix4x4f m_cViewMatrix;
};

#endif