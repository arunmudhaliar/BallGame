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

	m_cBall.load(getResourcePath("mesh//sphere.mesh"), getResourcePath("textures//checkerz.png"), m_cTextureManager);
	m_cTargetTrailEffect.init(m_cTextureManager);
	m_cBorderWall.init(m_cTextureManager);
	m_cPathGenerator.init(m_cTextureManager);
	m_cBall.setColor(vector3f(1.0f, 1.0f, 1.0f));

	return true;
}

void gameScene::onSize(int cx, int cy)
{
	getCommonData()->getRendererPtr()->setViewPort(0, 0, cx, cy);
	m_cBorderWall.set2DPosition(cx*0.5f, cy*0.5f);
	m_cBall.set2DPosition(cx*0.5f, cy*0.5f);
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
	case State_Simulate:
	{
		doSimulate(dt);
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
	m_cTargetTrailEffect.drawTrail();

	m_cBall.render(*objectBase::getRenderer()->getViewMatrix());
	m_cPathGenerator.drawPath();
	glPopMatrix();

    char buffer[128];
	sprintf(buffer, "Click and drag inside the box.");
    getCommonData()->getArialBold15Font()->drawString(buffer, 10, 30, false, true);

	Scene::onRender();
}

void gameScene::onTouchBegin(int x, int y, void* touchPtr)
{
	//if the state is idle then start the path-generator logic.
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

	switch (m_eGameState)
	{
	case State_Idle:
	{
		//if the state is idle then calculate the trail to render below the ball.
		m_cTargetTrailEffect.calculateTrail(m_cBall.getPosition2(), vector2f(x+viewPos.x, y+viewPos.y));
		m_cTargetTrailEffect.setVisible(true);
		break;
	}
	case State_EditPath:
	{
		//if the state is 'editPath' then calculate the new point based on the rules.
		int flag = *(int*)touchPtr;
		if ((MK_LBUTTON&flag))
		{
			m_cPathGenerator.doPath(x+viewPos.x, y+viewPos.y);
		}
		break;
	}
	}
}

void gameScene::onTouchEnd(int x, int y, bool bProcessed, void* touchPtr)
{
	if (m_eGameState == State_EditPath)
	{
		//End the generation logic for path-generator.
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
		m_bCollisionOccuredOnWall=false;
		break;
	}
	case State_EditPath:
	{
		m_cTargetTrailEffect.setVisible(false);
		break;
	}
	case State_Simulate:
	{
		m_bStopFollowCam = false;
		break;
	}
	//case State_EndSimulation:
	//{
	//	break;
	//}
	}
}

void gameScene::doSimulate(float dt)
{
	auto oldPos = m_cBall.getPosition2();
	m_cBall.update(dt);

	//collision detection
	auto newPos = m_cBall.getPosition2();
	if(m_cBorderWall.checkCollision(newPos, m_cBall.getRadius()))
	{
		m_bCollisionOccuredOnWall = true;
		m_cBall.turnRed();
	}

	//if ball collided with the wall, then the ball will not proceed on the designed path. 
	//It will stop after a while.
	if(m_bCollisionOccuredOnWall)
	{
		m_cPathGenerator.popTop(m_cPathGenerator.getPathCount());
		float velocity = m_cBall.getVelocity().length();
		if(velocity<0.0001f)
		{
			setGameState(State_Idle);
			m_bCollisionOccuredOnWall=false;
		}
	}

	//set the new position as per the physics.
	m_cBall.set2DPosition(newPos.x, newPos.y);

	//check if the ball reached the top point in path
	if (!m_cPathGenerator.isAnyPath()) return;

	//calculate the force on the basis of PATH_GENERATOR_NO_OF_POINTS_TO_APPROXIMATE from the path list.
	auto targetPos = m_cPathGenerator.getTop(PATH_GENERATOR_NO_OF_POINTS_TO_APPROXIMATE);
	auto ballPos = m_cBall.getPosition2();
	float t = 0.0f;
	if (m_cPathGenerator.isReachedNearTop(ballPos, BALL_RADIUS, t, PATH_GENERATOR_NO_OF_POINTS_TO_APPROXIMATE))
	{
		m_cPathGenerator.popTop(PATH_GENERATOR_NO_OF_POINTS_TO_APPROXIMATE);
		if (m_cPathGenerator.getPathCount()==0)
		{
			setGameState(State_Idle);
		}
	}
	else
	{
		//calculate the force towards the calculated point.
		auto diff = targetPos - ballPos;
		diff.normalize();
		diff = diff*BALL_SPEED;
		m_cBall.addForce(diff);
	}
}

//chase cam
void gameScene::followObject(float dt, objectBase* chasedObj)
{
	if(dt>0.1f || m_bStopFollowCam) return;
	if(chasedObj==NULL) return;

	matrix4x4f* chasingObj=objectBase::getRenderer()->getViewMatrix();

	auto viewportSz = getCommonData()->getRendererPtr()->getViewPortSz();
	vector3f	eyeOff(-viewportSz.x*0.5f, -viewportSz.y*0.5f, -BALL_RADIUS*0.5f);

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