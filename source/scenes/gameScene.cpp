#include "gameScene.h"
#include "../engine/core/Timer.h"

gameScene::gameScene() :
Scene(1)
{
    m_pTouchPtr=NULL;
}

gameScene::~gameScene()
{
}

void gameScene::onInit()
{
	Scene::onInit();
    
    setFaderRGB(1, 1, 1);
    setFaderAlpha(1, 0.5f);
    setFaderSpeed(0.5f);
    startFader(300);
    
    m_pTouchPtr=NULL;
    loadResource(0);
}

bool gameScene::loadResource(int userdefined)
{
	memset(m_bszKeyBuffers, 0, sizeof(m_bszKeyBuffers));

	m_cViewMatrix.identity();

	m_cBall.load(getResourcePath("mesh//cube.preview"));
	m_cTargetTrailEffect.init(m_cTextureManager);
	m_cBorderWall.setOffset(0, 0);
	m_cBorderWall.loadTexture(&m_cTextureManager, getResourcePath("textures//circleoffire.png"));
	m_cBorderWall.setScale(10, 10, 1);
	m_cBorderWall.recalculateTile();

	m_cEntityManager.init(this);
	m_cPathGenerator.init(m_cTextureManager);

	getCommonData()->getPlayerData()->resetScores();

	return true;
}

void gameScene::onReloadOpneGLResources()
{
    Scene::onReloadOpneGLResources();
}

void gameScene::onSize(int cx, int cy)
{
	getCommonData()->getRendererPtr()->setViewPort(0, 0, cx, cy);

	auto sz = getCommonData()->getRendererPtr()->getViewPortSz();
	m_cBorderWall.set2DPosition(sz.x*0.5f, sz.y*0.5f);
	m_cBall.set2DPosition(sz.x*0.5f, sz.y*0.5f);

	Scene::onSize(cx, cy);
}

void gameScene::onFixedUpdate(float dt)
{
    Scene::onFixedUpdate(dt);
}

void gameScene::onAnimationStart(unsigned int msg, float scale)
{
}

void gameScene::onAnimation(unsigned int msg, float scale)
{
}

void gameScene::onAnimationEnd(unsigned int msg, float scale)
{
}

void gameScene::onUpdate(unsigned int dtm)
{
	Scene::onUpdate(dtm);
    if(isPaused()) return;
        
    float dt=(float)dtm/1000.0f;
    dt=min(dt, 0.05f);

	processKeyInputs();

	m_cBall.update(dt);
	m_cTargetTrailEffect.update(dt);
	m_cEntityManager.update(dt);

	switch (m_eGameState)
	{
	case State_Idle:
	{
		break;
	}
	case State_EditPath:
	{
		break;
	}
	case State_Simulate:
	{
		m_cBall.updatePhysics(dt);

		//check if the ball reached the top point in path
		if (!m_cPathGenerator.isAnyPath()) break;


		break;
	}
	case State_EndSimulation:
	{
		break;
	}
	}
}

void gameScene::onRender()
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
	m_cEntityManager.render(*objectBase::getRenderer()->getViewMatrix());
	m_cBorderWall.draw(*objectBase::getRenderer()->getViewMatrix());
	m_cBall.render(*objectBase::getRenderer()->getViewMatrix());
	m_cPathGenerator.drawPath();
	m_cTargetTrailEffect.drawTrail();
	glPopMatrix();

    char buffer[128];
	sprintf(buffer, "fps = %2.2f\n\n\nSCORE = %d / %d\nrot=%d", 
					Timer::getFPS(), 
					getCommonData()->getPlayerData()->m_iCurrentScore,
					getCommonData()->getPlayerData()->m_iHighScore,
					m_cPathGenerator.getPathCount());

    getCommonData()->getArialBold15Font()->drawString(buffer, 10, 30, false, true);

	Scene::onRender();
}

void gameScene::onExit()
{
	m_cEntityManager.reset();

	Scene::onExit();
}

void gameScene::onPause()
{
	Scene::onPause();
}

void gameScene::onResume()
{
    //resumeWithAllChilds();
	resume();
	Scene::onResume();
}

void gameScene::onTimer(unsigned int msg)
{
}

void gameScene::onFaderComplete(unsigned int msg)
{
}

void gameScene::processKeyInputs()
{
	if(m_bszKeyBuffers[38])	//up
	{
	}

	if(m_bszKeyBuffers[37])	//left
	{
	}

	if(m_bszKeyBuffers[39])	//right
	{
	}

	if(m_bszKeyBuffers[32])
	{
	}
}

void gameScene::onPreUpdateEntity(entityBase* entity)
{
}

void gameScene::onPostUpdateEntity(entityBase* entity)
{
}

void gameScene::onKeyDown(int keyCode)
{
	m_bszKeyBuffers[keyCode]=true;
}

void gameScene::onKeyUp(int keyCode)
{
	m_bszKeyBuffers[keyCode]=false;
}

void gameScene::onTouchBegin(int x, int y, void* touchPtr)
{
	if (m_eGameState == State_Idle)
	{
		setGameState(State_EditPath);
		m_cPathGenerator.doBeginPath();
		m_cPathGenerator.doPath(x, y);
	}
}

void gameScene::onTouchMoved(int x, int y, void* touchPtr)
{
	if (m_eGameState == State_Idle)
	{
		m_cTargetTrailEffect.calculateTrail(m_cBall.getPosition2(), vector2f(x, y));
	}

	if (m_eGameState == State_EditPath)
	{
		int flag = *(int*)touchPtr;
		if ((MK_LBUTTON&flag))
		{
			m_cPathGenerator.doPath(x, y);
		}
	}
}

void gameScene::onTouchEnd(int x, int y, bool bProcessed, void* touchPtr)
{
	if (m_eGameState == State_EditPath)
	{
		m_cPathGenerator.doPath(x, y);
		m_cPathGenerator.doEndPath();
		setGameState(State_Simulate);
	}
}

void gameScene::setGameState(kGameState state)
{
	if (m_eGameState != state)
	{
		m_eGameState = state;
		onGameStateChange();
	}
}

void gameScene::onGameStateChange()
{
	switch (m_eGameState)
	{
	case State_Idle:
	{
		break;
	}
	case State_EditPath:
	{
		break;
	}
	case State_Simulate:
	{
		break;
	}
	case State_EndSimulation:
	{
		break;
	}
	}
}
