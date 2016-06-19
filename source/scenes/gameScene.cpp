#include "gameScene.h"
#include "../engine/core/Timer.h"

gameScene::gameScene() :
Scene(1)
{
	m_bStopFollowCam = false;
	m_bCollisionOccuredOnWall = false;
	setGameState(State_Idle);
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
    
    loadResource(0);
}

bool gameScene::loadResource(int userdefined)
{
	m_cViewMatrix.identity();

	m_cBall.load(getResourcePath("mesh//cube.preview"));
	m_cTargetTrailEffect.init(m_cTextureManager);
	m_cBorderWall.init(m_cTextureManager);
	m_cPathGenerator.init(m_cTextureManager);
	return true;
}

void gameScene::onSize(int cx, int cy)
{
	getCommonData()->getRendererPtr()->setViewPort(0, 0, cx, cy);

	auto sz = getCommonData()->getRendererPtr()->getViewPortSz();
	m_cBorderWall.set2DPosition(sz.x*0.5f, sz.y*0.5f);
	m_cBall.set2DPosition(sz.x*0.5f, sz.y*0.5f);
	m_bStopFollowCam = false;

	Scene::onSize(cx, cy);
}

void gameScene::onUpdate(unsigned int dtm)
{
	Scene::onUpdate(dtm);
    if(isPaused()) return;
        
    float dt=(float)dtm/1000.0f;
    dt=min(dt, 0.05f);

	m_cBall.update(dt);
	m_cTargetTrailEffect.update(dt);

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
		auto oldPos = m_cBall.getPosition2();
		m_cBall.updatePhysics(dt);

		//collision detection
		auto newPos = m_cBall.getPosition2();
		if(m_cBorderWall.checkCollision(newPos))
		{
			m_bCollisionOccuredOnWall = true;
			m_fElapsedTimeAfterFirstCollision=0.0f;
		}

		if(m_bCollisionOccuredOnWall)
		{
			m_cPathGenerator.popTop(m_cPathGenerator.getPathCount());
			auto diff = (oldPos-newPos).length();
			if(diff<0.00001f)
			{
				m_fElapsedTimeAfterFirstCollision+=dt;
				if(m_fElapsedTimeAfterFirstCollision>0.25f)
				{
					setGameState(State_Idle);
					m_bCollisionOccuredOnWall=false;
				}
			}
		}

		m_cBall.set2DPosition(newPos.x, newPos.y);

		//check if the ball reached the top point in path
		if (!m_cPathGenerator.isAnyPath()) break;

		auto targetPos = m_cPathGenerator.getTop(PATH_AVG_COUNT);
		auto ballPos = m_cBall.getPosition2();
		float t = 0.0f;
		if (m_cPathGenerator.isReachedNearTop(ballPos, 5.0f, t, PATH_AVG_COUNT))
		{
			m_cPathGenerator.popTop(PATH_AVG_COUNT);
			if (m_cPathGenerator.getPathCount()==0)
			{
				setGameState(State_Idle);
			}
		}
		else
		{
			auto diff = targetPos - ballPos;
			diff.normalize();
			diff = diff*BALL_SPEED;
			m_cBall.addForce(diff);
		}
		break;
	}
	case State_EndSimulation:
	{
		break;
	}
	}

	followObject(dt, &m_cBall);
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
	glMultMatrixf(objectBase::getRenderer()->getViewMatrix()->getInverse().getMatrix());
	m_cBorderWall.drawWall();
	if(m_eGameState==State_Idle)
	{
		m_cTargetTrailEffect.drawTrail();
	}

	m_cBall.render(*objectBase::getRenderer()->getViewMatrix());
	m_cPathGenerator.drawPath();
	glPopMatrix();

    char buffer[128];
	sprintf(buffer, "fps = %2.2f\n\n\nSCORE = %d / %d\npath=%d", 
					Timer::getFPS(), 
					getCommonData()->getPlayerData()->m_iCurrentScore,
					getCommonData()->getPlayerData()->m_iHighScore,
					m_cPathGenerator.getPathCount());

    getCommonData()->getArialBold15Font()->drawString(buffer, 10, 30, false, true);

	Scene::onRender();
}

void gameScene::onTouchBegin(int x, int y, void* touchPtr)
{
	if (m_eGameState == State_Idle)
	{
		setGameState(State_EditPath);
		m_cPathGenerator.doBeginPath();

		matrix4x4f* view = objectBase::getRenderer()->getViewMatrix();
		auto viewPos = view->getPosition2();
		m_cPathGenerator.doPath(x+viewPos.x, y+viewPos.y);
	}
}

void gameScene::onTouchMoved(int x, int y, void* touchPtr)
{
	matrix4x4f* view = objectBase::getRenderer()->getViewMatrix();
	auto viewPos = view->getPosition2();

	if (m_eGameState == State_Idle)
	{
		m_cTargetTrailEffect.calculateTrail(m_cBall.getPosition2(), vector2f(x+viewPos.x, y+viewPos.y));
	}

	if (m_eGameState == State_EditPath)
	{
		int flag = *(int*)touchPtr;
		if ((MK_LBUTTON&flag))
		{
			m_cPathGenerator.doPath(x+viewPos.x, y+viewPos.y);
		}
	}
}

void gameScene::onTouchEnd(int x, int y, bool bProcessed, void* touchPtr)
{
	if (m_eGameState == State_EditPath)
	{
		matrix4x4f* view = objectBase::getRenderer()->getViewMatrix();
		auto viewPos = view->getPosition2();

		m_cPathGenerator.doPath(x+viewPos.x, y+viewPos.y);
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
		m_bStopFollowCam = false;
		break;
	}
	case State_EndSimulation:
	{
		break;
	}
	}
}

void gameScene::resetCollisionVars()
{
	m_bCollisionOccuredOnWall = false;
}

//chase cam
void gameScene::followObject(float dt, objectBase* chasedObj)
{
	if(dt>0.1f || m_bStopFollowCam) return;
	if(chasedObj==NULL) return;

	matrix4x4f* chasingObj=objectBase::getRenderer()->getViewMatrix();

	auto viewportSz = getCommonData()->getRendererPtr()->getViewPortSz();
	vector3f	eyeOff(-viewportSz.x*0.5f, -viewportSz.y*0.5f, -5);

	vector3f    transformedEye((chasedObj->getPosition()) + eyeOff);
	vector3f    chasingObjPos(chasingObj->getPosition());
	vector3f    lenV(transformedEye-chasingObjPos);
	float       len=lenV.length();

	if(len==0.0f)
		return;

	if(len<=0.01f)
	{
		m_bStopFollowCam=true;
		return;
	}

	if(len>4000.0f)
	{
		float factor=4000.0f/len;
		lenV=lenV*factor;
	}

	float speed=10.0f;
	vector3f    updatedPos(chasingObjPos+lenV*(speed*dt));
	chasingObj->setPosition(updatedPos);
}