#include "SceneManager.h"
#include "../util/gxDebug.h"
#include "../core/Timer.h"

SceneManager::SceneManager()
{
	m_pActiveScene = NULL;
    m_pActiveSceneAfterThisUpdate = NULL;
    m_pPushMeToActiveSceneAfterThisUpdate = NULL;
	m_cTimerInterval=0;
	m_cElapsedTimer=0;
	m_cTimerMsg=0;
	m_bCallTimerImmediate=false;
	m_nTimerCallbackCount=0;
	disableTimer();
	m_cScreenWidth=0;
	m_cScreenHeight=0;
    m_uElapsedTimeAfterPreviousFixedUpdate=0;
    m_bPause=false;
}

SceneManager::~SceneManager()
{
	m_cszSceneList.clearAll();
}

void SceneManager::init()
{
}

void SceneManager::setActiveScene(Scene* scene)
{
	clearTimerVars();

	if(m_pActiveScene) m_pActiveScene->onExit();
	m_pActiveScene=scene;
	m_pActiveScene->onInit();
	m_pActiveScene->onSize(m_cScreenWidth, m_cScreenHeight);
}

void SceneManager::setActiveSceneAfterThisUpdate(Scene* scene)
{
    m_pActiveSceneAfterThisUpdate=scene;
}

void SceneManager::pushToActiveSceneAfterThisUpdate(Scene* scene)
{
    m_pPushMeToActiveSceneAfterThisUpdate=scene;
}

Scene* SceneManager::getActiveScene()
{
	return m_pActiveScene;
}

void SceneManager::setTimerInterval(int interval)
{
	m_cTimerInterval=interval;
	m_cElapsedTimer=interval;
}

void SceneManager::setTimerMsg(unsigned int msg)
{
	m_cTimerMsg=msg;
}

void SceneManager::setCallTimerImmediate(bool bFlag)
{
	m_bCallTimerImmediate=bFlag;
}

void SceneManager::setTimerRepeate(bool bRepeate)
{
	m_bTimerRepeate=bRepeate;
}

void SceneManager::setTimerCallbackCount(unsigned int count)
{
	m_nTimerCallbackCount=count;
}

void SceneManager::enableTimer()
{
	m_bTimer=true;
}

void SceneManager::disableTimer()
{
	m_bTimer=false;
	clearTimerVars();
}

void SceneManager::clearTimerVars()
{
	m_cTimerInterval=0;
	m_cElapsedTimer=0;
	m_cTimerMsg=0;
	m_bCallTimerImmediate=false;
	m_nTimerCallbackCount=0;
    
    m_uElapsedTimeAfterPreviousFixedUpdate=0;
}

void SceneManager::update(unsigned int dtm)
{
	if(!getActiveScene()) return;

    getActiveScene()->onFixedUpdate((float)dtm/1000.0f);
    getActiveScene()->onUpdate(dtm);

	//timer func
	if(m_bTimer)
	{
		if(m_bCallTimerImmediate)
		{
			getActiveScene()->onTimer(m_cTimerMsg);
			m_cElapsedTimer-=dtm;
			m_bCallTimerImmediate=false;
			if(m_nTimerCallbackCount)
			{
				m_nTimerCallbackCount--;
				if(m_nTimerCallbackCount==0)disableTimer();
			}
		}
		else
		{
			m_cElapsedTimer-=dtm;
			if(m_cElapsedTimer<=0)
			{
				getActiveScene()->onTimer(m_cTimerMsg);
				if(m_bTimerRepeate)
					m_cElapsedTimer=m_cTimerInterval;
				else
				{
					if(m_nTimerCallbackCount)
					{
						m_nTimerCallbackCount--;
						m_cElapsedTimer=m_cTimerInterval;
						if(m_nTimerCallbackCount==0)disableTimer();
					}
					else
						disableTimer();
				}
			}
		}
	}
	//
    
    //if(m_pPushMeToActiveSceneAfterThisUpdate)
    //{
    //    getActiveScene()->pushScene(m_pPushMeToActiveSceneAfterThisUpdate);
    //    m_pPushMeToActiveSceneAfterThisUpdate=NULL;
    //}
    
    if(m_pActiveSceneAfterThisUpdate)
    {
        setActiveScene(m_pActiveSceneAfterThisUpdate);
        m_pActiveSceneAfterThisUpdate=NULL;
    }
}

void SceneManager::render()
{
	if(getActiveScene()) getActiveScene()->onRender();
}

void SceneManager::resize(unsigned int cx, unsigned int cy)
{
	m_cScreenWidth=cx;
	m_cScreenHeight=cy;
	if(getActiveScene()) getActiveScene()->onSize(cx, cy);
}

void SceneManager::pause()
{
    m_bPause=true;
	if(getActiveScene())
		getActiveScene()->onPause();
}

void SceneManager::reloadOpenGLResource()
{
    for(int x=0;x<(int)m_cszSceneList.getSize();x++)
	{
        m_cszSceneList[x]->onReloadOpneGLResources();
	}
}

void SceneManager::resume()
{
    m_bPause=false;
	if(getActiveScene())
		getActiveScene()->onResume();
}
	
void SceneManager::appendScene(Scene* scene)
{
	scene->setSceneManager(this);
	m_cszSceneList.insertTail(scene);
}

bool SceneManager::removeScene(Scene* scene)
{
	for(int x=0;x<(int)m_cszSceneList.getSize();x++)
	{
		if(m_cszSceneList[x]==scene)
		{
			m_cszSceneList[x]->onExit();
			m_cszSceneList.removeAt(x);
			return true;
		}
	}
	
	return false;
}

Scene* SceneManager::removeScene(unsigned int index)
{
	if(index>=(unsigned int)m_cszSceneList.getSize()) return NULL;
	
	Scene* scene=m_cszSceneList[index];
	scene->onExit();
	m_cszSceneList.removeAt(index);
	
	return scene;
}

void SceneManager::destroyAll()
{
	for(int x=0;x<(int)m_cszSceneList.getSize();x++)
	{
		m_cszSceneList[x]->onExit();
		Scene* scene=m_cszSceneList[x];
		GX_DELETE(scene);
	}
	
	m_pActiveScene=NULL;
    m_pActiveSceneAfterThisUpdate=NULL;
    m_pPushMeToActiveSceneAfterThisUpdate=NULL;
	m_cszSceneList.clearAll();
}

Scene* SceneManager::getSceneByID(unsigned int sceneID)
{
	for(int x=0;x<(int)m_cszSceneList.getSize();x++)
	{
		if(m_cszSceneList[x]->getSceneID()==sceneID)
		{
			return m_cszSceneList[x];
		}
	}
	
	return NULL;	
}

Scene* SceneManager::getSceneByIndex(unsigned int index)
{
	if(index>=(unsigned int)m_cszSceneList.getSize()) return NULL;

	return m_cszSceneList[index];
}

void SceneManager::touchBegin(int x, int y, void* touchPtr)
{
	if(getActiveScene())
    {
        Scene* topscene=getActiveScene()->getSceneOnTop();
        topscene->passTouchEvents();
        topscene->onTouchBegin(x, y, touchPtr);
    }
}

void SceneManager::touchMoved(int x, int y, void* touchPtr)
{
	if(getActiveScene())
    {
        Scene* topscene=getActiveScene()->getSceneOnTop();
        if(!topscene->isPassTouchEventsEnabled()) return;
        topscene->onTouchMoved(x, y, touchPtr);
    }
}

void SceneManager::doubleTap(int x, int y, void* touchPtr)
{
	if(getActiveScene())
    {
        Scene* topscene=getActiveScene()->getSceneOnTop();
        if(!topscene->isPassTouchEventsEnabled()) return;
        topscene->onDoubleTap(x, y, touchPtr);
    }
}

void SceneManager::touchEnd(int x, int y, void* touchPtr)
{
	if(getActiveScene())
    {
        Scene* topscene=getActiveScene()->getSceneOnTop();
        Scene* parent=topscene->getParent();
        topscene->onTouchEnd(x, y, false, touchPtr);
    }
}

void SceneManager::keyDown(int keyCode)
{
	if(getActiveScene()) getActiveScene()->getSceneOnTop()->onKeyDown(keyCode);
}

void SceneManager::keyUp(int keyCode)
{
	if(getActiveScene()) getActiveScene()->getSceneOnTop()->onKeyUp(keyCode);
}
