#include "gameScene.h"
#include "../engine/core/Timer.h"

gameScene::gameScene() :
Scene(1)
{
    m_pTouchPtr=NULL;
	m_bStopFollowCam = false;
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
    
    m_pTouchPtr=NULL;
    loadResource(0);
}

bool gameScene::loadResource(int userdefined)
{
	memset(m_bszKeyBuffers, 0, sizeof(m_bszKeyBuffers));

	m_cViewMatrix.identity();

	m_cBall.load(getResourcePath("mesh//cube.preview"));
	m_cTargetTrailEffect.init(m_cTextureManager);
	m_cBorderWall.init(m_cTextureManager);
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
	m_bStopFollowCam = false;

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
	//m_cEntityManager.render(*objectBase::getRenderer()->getViewMatrix());
	m_cBorderWall.drawWall();
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

    //getCommonData()->getArialBold15Font()->drawString(buffer, 10, 30, false, true);

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

#include "../engine/util/util.h"

void checkCollision(vector2f newPos)
{
	int collision_check_cntr = 5;
	while (collision_check_cntr--)
	{
		//oldPos = newPos;
		vector2f avgPos;
		int cnt = 0;
		float ACTOR_COLLISION_RADIUS = 10;
		float m_xActorRadiusSq = 10 * 10;
		//circle-rectangle collision
		vector2f left;// (tile->getLeftWorld());
		vector2f right;// (tile->getRightWorld());
		vector2f top;// (tile->getTopWorld());
		vector2f bottom;// (tile->getBottomWorld());

		//check for penitration
		bool bPenitration = true;
		if (newPos.x<left.x || newPos.y<top.y || newPos.x>right.x || newPos.y>bottom.y)
			bPenitration = false;

		//                    if(bPenitration)
		//                    {
		//                        DEBUG_PRINT("=======================PENITRATION OCCURED=================");
		//                    }

		vector2f closestPt[4];

		closestPt[0] = (gxUtil::closestPointOnLine(newPos, left, top));
		closestPt[1] = (gxUtil::closestPointOnLine(newPos, top, right));
		closestPt[2] = (gxUtil::closestPointOnLine(newPos, right, bottom));
		closestPt[3] = (gxUtil::closestPointOnLine(newPos, bottom, left));

		//which one is the closest
		int closest_length = GX_MAX_INT;
		int closest_index = -1;

		for (int l = 0; l < 4; l++)
		{
			vector2f diff(newPos - closestPt[l]);
			__int64 length = diff.lengthSquared();
			if (length < closest_length)
			{
				closest_length = length;
				closest_index = l;

				if (bPenitration)
				{
					diff = -diff;
				}

				if (closest_length <= m_xActorRadiusSq)
				{
					//collision occured
					diff.normalize();
					float val = ACTOR_COLLISION_RADIUS + 0.1f;
					vector2f calc_Pos(closestPt[closest_index] + (diff*val));
					avgPos += calc_Pos;
					cnt++;
					//DEBUG_PRINT("(%d), closestPt(%f, %f), calc_pos(%f, %f)", l, closestPtf[closest_index].x, closestPtf[closest_index].y, calc_Pos.x, calc_Pos.y);
				}
			}
		}//for

		if (cnt)
		{
			avgPos.x = avgPos.x / cnt;
			avgPos.y = avgPos.y / cnt;
			newPos = avgPos;	//this will cause the actor to come to a halt then move away, so i commented this line
			//DEBUG_PRINT("collision occured %d, avgPos(%f, %f)", cnt, avgPosf.x, avgPosf.x);
		}
		else
		{
			break;	//break the loop
		}
	}
}

//chase cam

void gameScene::followObject(float dt, objectBase* chasedObj)
{
	//return;

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