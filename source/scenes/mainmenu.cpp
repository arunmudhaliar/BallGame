#include "mainmenu.h"

mainMenu::mainMenu():
Scene(0)
{
}

mainMenu::~mainMenu()
{
}

void mainMenu::onInit()
{
	Scene::onInit();
    
    setFaderRGB(0, 0, 0);
    setFaderAlpha(1, 0);
    setFaderSpeed(0.5f);
    startFader(400);
    
    loadResource(0);
}

bool mainMenu::loadResource(int userdefined)
{
	m_cViewMatrix.identity();
	m_cViewMatrix.inverse();

	getCommonData()->getPlayerData()->readFromPref();

	return true;
}

void mainMenu::onReloadOpneGLResources()
{
    Scene::onReloadOpneGLResources();
}

void mainMenu::onSize(int cx, int cy)
{
	getCommonData()->getRendererPtr()->setViewPort(0, 0, cx, cy);

	Scene::onSize(cx, cy);
}

void mainMenu::onFixedUpdate(float dt)
{
    Scene::onFixedUpdate(dt);
}

void mainMenu::onAnimationStart(unsigned int msg, float scale)
{
}

void mainMenu::onAnimation(unsigned int msg, float scale)
{
}

void mainMenu::onAnimationEnd(unsigned int msg, float scale)
{
}

void mainMenu::onUpdate(unsigned int dtm)
{
	Scene::onUpdate(dtm);
    if(isPaused()) return;
        
    float dt=(float)dtm/1000.0f;
    if(dt>0.3f)dt=0.06f;

	processKeyInputs();
}

void mainMenu::onRender()
{    
	objectBase::getRenderer()->setViewMatrixToGL(&m_cViewMatrix);

#if !defined (USE_ProgrammablePipeLine)
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(objectBase::getRenderer()->getOrthoProjectionMatrix()->getMatrix());
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, 0, -1);
#endif
    
	glPushMatrix();
	glPopMatrix();

    //char buffer[128];
	//sprintf(buffer, "BALL GAME\n\n\n\nPress 'Space' to play the game... 'ESc' to exit.");
	//getCommonData()->getArialBold15Font()->drawString(buffer, getCommonData()->getScreenWidth()*0.5f, getCommonData()->getScreenHeight()*0.5f, true);

	Scene::onRender();
}

void mainMenu::onExit()
{
	Scene::onExit();
}

void mainMenu::onPause()
{
	Scene::onPause();
}

void mainMenu::onResume()
{
    //resumeWithAllChilds();
	resume();
	Scene::onResume();
}

void mainMenu::onTimer(unsigned int msg)
{
}

void mainMenu::onFaderComplete(unsigned int msg)
{
}

void mainMenu::processKeyInputs()
{
}

void mainMenu::onKeyDown(int keyCode)
{
}

void mainMenu::onKeyUp(int keyCode)
{
	if(keyCode==32)
	{
		getSceneManager()->setActiveSceneAfterThisUpdate(getSceneManager()->getSceneByID(1));
	}
}

void mainMenu::onTouchBegin(int x, int y, void* touchPtr)
{
}

void mainMenu::onTouchMoved(int x, int y, void* touchPtr)
{
}

void mainMenu::onTouchEnd(int x, int y, bool bProcessed, void* touchPtr)
{
}