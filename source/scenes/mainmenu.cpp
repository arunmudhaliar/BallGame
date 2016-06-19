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

void mainMenu::onSize(int cx, int cy)
{
	getCommonData()->getRendererPtr()->setViewPort(0, 0, cx, cy);

	Scene::onSize(cx, cy);
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

    char buffer[128];
	sprintf(buffer, "BALL GAME\n\n\n\nPress 'Space' to play the game... 'ESc' to exit.");
	getCommonData()->getArialBold15Font()->drawString(buffer, getCommonData()->getScreenWidth()*0.5f, getCommonData()->getScreenHeight()*0.5f, true);

	Scene::onRender();
}

void mainMenu::onKeyUp(int keyCode)
{
	if(keyCode==32)
	{
		getSceneManager()->setActiveSceneAfterThisUpdate(getSceneManager()->getSceneByID(1));
	}
}

void mainMenu::onChildPoped(Scene* childscene, int msg)
{
}
