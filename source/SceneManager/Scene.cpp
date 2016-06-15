#include "Scene.h"
#include "../engine/core/Timer.h"

CommonData	Scene::g_cCommonData;

Scene::Scene(unsigned int sceneID)
{
	m_cSceneID=sceneID;
	m_pSceneManagerPtr=NULL;
    m_bPause=false;
    setParent(NULL);
    passTouchEvents();
    //m_pPushMeAfterThisUpdatePtr=NULL;
    m_bPopTheTopAfterUpdate=false;
    m_iPopTheTopAfterUpdateMsg=0;
    m_fAnimationScale=0.0f;
    m_fAnimationSpeed=1.0f;
    m_iAnimationMsg=0;
    m_bAnimation=false;
    m_bIsSoundEnginePausedOnPreviousState=false;
}

Scene::~Scene()
{    
#if defined (LOG_DEBUG_ENGINE)
    DEBUG_PRINT("Scene Destroyed %d", m_cSceneID);
#endif 
}

void Scene::onInit()
{
	initFader(Scene::getCommonData());
    //clearPopAfterThisUpdate();
}

void Scene::onReloadOpneGLResources()
{
}

void Scene::onSize(int cx, int cy)
{
}

void Scene::onFixedUpdate(float dt)
{
    //stLinkNode<Scene*>* node=m_cszChildSceneList.getHead();
    //while(node)
    //{
    //    node->getData()->onFixedUpdate(dt);
    //    node=node->getNext();
    //}
}

void Scene::onUpdate(unsigned int dtm)
{
    //setTouchReceivedForThisUpdate(false);
    
	updateFader(Timer::getDtinSec());

    if(m_bAnimation && !m_bPause)
    {
        m_fAnimationScale+=Timer::getDtinSec()*m_fAnimationSpeed;
        if(m_fAnimationScale>1.0f)
        {
            m_fAnimationScale=1.0f;
            m_bAnimation=false;
            onAnimation(m_iAnimationMsg, m_fAnimationScale);
        }
        if(m_bAnimation)
            onAnimation(m_iAnimationMsg, m_fAnimationScale);
        else
            onAnimationEnd(m_iAnimationMsg, m_fAnimationScale);
    }
}

void Scene::onRender()
{
  //  m_cInput.debugDraw();
	renderFader();
}

void Scene::onExit()
{
   
    m_fAnimationScale=1.0f;
    m_iAnimationMsg=0;
    m_bAnimation=false;
}

void Scene::onPause()
{
//    m_bIsSoundEnginePausedOnPreviousState=getCommonData()->getSoundEngine()->isPaused();
    
//    getCommonData()->getSoundEngine()->pause();
    pause();
}

void Scene::onResume()
{
    //if(getSceneManager()->getActiveScene()->shouldResumeSoundEngine())
    //{
    //    if(!m_bIsSoundEnginePausedOnPreviousState)
    //    {
    //        getCommonData()->getSoundEngine()->resume();
    //    }
    //    else
    //    {
    //        m_bIsSoundEnginePausedOnPreviousState=false;
    //    }
    //}
}

void Scene::onTimer(unsigned int msg)
{
}

bool Scene::setSceneManager(SceneManager* sceneManager)
{
	m_pSceneManagerPtr=sceneManager;
	if(sceneManager) return true;
	return false;
}

SceneManager* Scene::getSceneManager()
{
	return m_pSceneManagerPtr;
}

bool Scene::setTimer(int interval, unsigned int msg, bool bRepeate, bool bImmediate)
{
	if(getSceneManager()->getActiveScene()!=this) return false;
	
	getSceneManager()->setTimerInterval(interval);
	getSceneManager()->setTimerMsg(msg);
	getSceneManager()->setTimerRepeate(bRepeate);
	getSceneManager()->setCallTimerImmediate(bImmediate);
	getSceneManager()->setTimerCallbackCount(0);
	getSceneManager()->enableTimer();
	
	return true;
}

bool Scene::setTimerWithCount(int interval, unsigned int msg, unsigned int count, bool bImmediate)
{
	if(getSceneManager()->getActiveScene()!=this) return false;
	
	getSceneManager()->setTimerInterval(interval);
	getSceneManager()->setTimerMsg(msg);
	getSceneManager()->setTimerRepeate(false);
	getSceneManager()->setCallTimerImmediate(bImmediate);
	getSceneManager()->setTimerCallbackCount(count);
	getSceneManager()->enableTimer();
	
	return true;
}

bool Scene::resetTimer()
{
	if(getSceneManager()->getActiveScene()!=this) return false;

	getSceneManager()->setTimerInterval(0);
	getSceneManager()->setTimerMsg(0);
	getSceneManager()->setTimerRepeate(false);
	getSceneManager()->setCallTimerImmediate(false);
	getSceneManager()->setTimerCallbackCount(0);
	getSceneManager()->disableTimer();
	
	return true;
}

void Scene::onTouchBegin(int x, int y, void* touchPtr)
{
}

void Scene::onTouchMoved(int x, int y, void* touchPtr)
{
}

void Scene::onTouchEnd(int x, int y, bool bProcessed, void* touchPtr)
{
}

void Scene::onDoubleTap(int x, int y, void* touchPtr)
{
}

void Scene::onKeyDown(int keyCode)
{
}

void Scene::onKeyUp(int keyCode)
{
}

void Scene::onAnimationStart(unsigned int msg, float scale)
{
    
}

void Scene::onAnimation(unsigned int msg, float scale)
{
    
}

void Scene::onAnimationEnd(unsigned int msg, float scale)
{
    
}

