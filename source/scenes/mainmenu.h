#ifndef MAINMENU_H
#define MAINMENU_H

#include "../SceneManager/Scene.h"

class mainMenu : public Scene
{
public:
	mainMenu();
	~mainMenu();
	
protected:
	//scene callbacks
	void onInit();
	void onSize(int cx, int cy);
	void onRender();
	void onKeyUp(int keyCode);
    bool loadResource(int userdefined);
	void onChildPoped(Scene* childscene, int msg);

private:
	matrix4x4f m_cViewMatrix;
};

#endif