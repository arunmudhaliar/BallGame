#ifndef SCENE_H
#define SCENE_H

#include "../core/pxMath.h"
#include "SceneManager.h"
#include "CommonData.h"
#include "SceneFader.h"
#include "../util/bxLinkedList.h"

class SceneManager;

class Scene : public SceneFader
{
public:
	Scene(unsigned int sceneID);
	virtual ~Scene();
	
    enum ESCENEANIMATION
    {
        ANIM_LAUNCHING  =   100,
        ANIM_CLOSING,
        ANIM_AUTOCLOSE
    };
    
	virtual void onInit();
	virtual void onSize(int cx, int cy);
	virtual void onUpdate(unsigned int dtm);
    virtual void onFixedUpdate(float dt);           //not yet implemented
	virtual void onRender();
	virtual void onExit();
	virtual void onPause();
	virtual void onResume();
	virtual void onTimer(unsigned int msg);
    
    virtual void onAnimationStart(unsigned int msg, float scale);   //called immediate after startAnimation()
    virtual void onAnimation(unsigned int msg, float scale);
    virtual void onAnimationEnd(unsigned int msg, float scale);
	
	virtual void onTouchBegin(int x, int y, void* touchPtr);
	virtual void onTouchMoved(int x, int y, void* touchPtr);
	virtual void onTouchEnd(int x, int y, bool bProcessed, void* touchPtr);
	virtual void onDoubleTap(int x, int y, void* touchPtr);

	virtual void onKeyDown(int keyCode);
	virtual void onKeyUp(int keyCode);
    
    virtual bool loadResource(int userdefined)=0;
    virtual void onChildPoped(Scene* childscene, int msg)=0;
    virtual void onReloadOpneGLResources();
    
    virtual bool shouldResumeSoundEngine()  {   return true;    }
	
	bool setSceneManager(SceneManager* sceneManager);
	SceneManager* getSceneManager();
	bool setTimer(int interval, unsigned int msg, bool bRepeate=true, bool bImmediate=false);
	bool setTimerWithCount(int interval, unsigned int msg, unsigned int count, bool bImmediate=false);
	bool resetTimer();
	unsigned int getSceneID()	{	return m_cSceneID;	}
	
	static CommonData* getCommonData()	{	return &g_cCommonData;	}
	static CommonData g_cCommonData;

    Scene* getSceneOnTop()                
    {
        return this;
    }

    //user is responsible for handling the logical part by using these functions
private:
    void pause()    {   m_bPause=true;      }
public:
    void resume()   {   m_bPause=false;     }
    bool isPaused() {   return m_bPause;    }
    //
    
    void setParent(Scene* scene)    {   m_pParentPtr=scene;     }
    Scene* getParent()              {   return m_pParentPtr;    }
    
    void passTouchEvents()          {   m_bCanPassTouchEvents=true;     }
    bool isPassTouchEventsEnabled() {   return m_bCanPassTouchEvents;   }
    void blockTouchEvents()         {   m_bCanPassTouchEvents=false;    }
    
    void setUserDefinedData(int data, int index)    {   m_iszUserDefinedInt[index]=data;    }
    int getUserDefinedData(int index)               {   return m_iszUserDefinedInt[index];  }
    
    void startAnimation(unsigned int msg, float scale=0.0f, float speed=1.0f)
    {   m_fAnimationScale=scale; m_fAnimationSpeed=speed; m_iAnimationMsg=msg; m_bAnimation=true;  onAnimationStart(m_iAnimationMsg, m_fAnimationScale);   }
    
    unsigned int getAnimationMessageID()    {   return m_iAnimationMsg; }
    
private:
	unsigned int m_cSceneID;
	SceneManager* m_pSceneManagerPtr;           //must not delete this pointer

    //pop after update
    bool m_bPopTheTopAfterUpdate;
    int m_iPopTheTopAfterUpdateMsg;
    
    Scene* m_pParentPtr;    //must not delete this pointer
    bool m_bPause;
    bool m_bCanPassTouchEvents;
    int m_iszUserDefinedInt[4];
    
    //animation
    bool m_bAnimation;
    float m_fAnimationScale;    //0-1
    float m_fAnimationSpeed;
    unsigned int m_iAnimationMsg;
    bool m_bIsSoundEnginePausedOnPreviousState;
};

#endif