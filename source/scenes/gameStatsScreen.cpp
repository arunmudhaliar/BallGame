#include "gameStatsScreen.h"

gameStatsScreen::gameStatsScreen():
Scene(2)
{
}

gameStatsScreen::~gameStatsScreen()
{
}

void gameStatsScreen::onInit()
{
	Scene::onInit();
    
    setFaderRGB(0, 0, 0);
    setFaderAlpha(1, 0);
    setFaderSpeed(0.5f);
    startFader(400);
    
    loadResource(0);
}

bool gameStatsScreen::loadResource(int userdefined)
{
	m_cViewMatrix.identity();
	m_cViewMatrix.inverse();

	getCommonData()->getPlayerData()->writeToPref();
	return true;
}

void gameStatsScreen::onReloadOpneGLResources()
{
    Scene::onReloadOpneGLResources();
}

void gameStatsScreen::onSize(int cx, int cy)
{
	getCommonData()->getRendererPtr()->setViewPort(0, 0, cx, cy);

	Scene::onSize(cx, cy);
}

void gameStatsScreen::onFixedUpdate(float dt)
{
    Scene::onFixedUpdate(dt);
}

void gameStatsScreen::onAnimationStart(unsigned int msg, float scale)
{
}

void gameStatsScreen::onAnimation(unsigned int msg, float scale)
{
}

void gameStatsScreen::onAnimationEnd(unsigned int msg, float scale)
{
}

void gameStatsScreen::onUpdate(unsigned int dtm)
{
	Scene::onUpdate(dtm);
    if(isPaused()) return;
        
    float dt=(float)dtm/1000.0f;
    if(dt>0.3f)dt=0.06f;

	processKeyInputs();
}

void gameStatsScreen::onRender()
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
	sprintf(buffer, "GAME STATS\n\n\nScore : %d\nHighScore : %d\n\n\nPress 'R' for replay... 'M' for main menu..",
		getCommonData()->getPlayerData()->m_iCurrentScore,
		getCommonData()->getPlayerData()->m_iHighScore);

	getCommonData()->getArialBold15Font()->drawString(buffer, getCommonData()->getScreenWidth()*0.5f, getCommonData()->getScreenHeight()*0.3f, true);

	Scene::onRender();
}

void gameStatsScreen::onExit()
{
	Scene::onExit();
}

void gameStatsScreen::onPause()
{
	Scene::onPause();
}

void gameStatsScreen::onResume()
{
    //resumeWithAllChilds();
	resume();
	Scene::onResume();
}

void gameStatsScreen::onTimer(unsigned int msg)
{
}

void gameStatsScreen::onFaderComplete(unsigned int msg)
{
}

void gameStatsScreen::processKeyInputs()
{
}

void gameStatsScreen::onKeyDown(int keyCode)
{
}

void gameStatsScreen::onKeyUp(int keyCode)
{
	if(keyCode==82)
		getSceneManager()->setActiveSceneAfterThisUpdate(getSceneManager()->getSceneByID(1));
	else if(keyCode==77)
		getSceneManager()->setActiveSceneAfterThisUpdate(getSceneManager()->getSceneByID(0));
}

void gameStatsScreen::onTouchBegin(int x, int y, void* touchPtr)
{
}

void gameStatsScreen::onTouchMoved(int x, int y, void* touchPtr)
{
}

void gameStatsScreen::onTouchEnd(int x, int y, bool bProcessed, void* touchPtr)
{
}