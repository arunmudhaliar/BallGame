#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Scene.h"
#include "../engine/util/bxLinkedList.h"

class Scene;

class SceneManager
{
public:
	SceneManager();
	~SceneManager();
	
	void init();
	void update(unsigned int dtm);
	void render();
	void resize(unsigned int cx, unsigned int cy);
	
	void pause();
	void resume();
    void reloadOpenGLResource();
	
	void touchBegin(int x, int y, void* touchPtr);
	void touchMoved(int x, int y, void* touchPtr);
	void touchEnd(int x, int y, void* touchPtr);
	void doubleTap(int x, int y, void* touchPtr);
	
	void keyDown(int keyCode);
	void keyUp(int keyCode);
	
	void appendScene(Scene* scene);
	bool removeScene(Scene* scene);
	Scene* removeScene(unsigned int index);
	void destroyAll();
	
	Scene* getSceneByID(unsigned int sceneID);
	Scene* getSceneByIndex(unsigned int index);
	
	void setActiveScene(Scene* scene);
	Scene* getActiveScene();
    void setActiveSceneAfterThisUpdate(Scene* scene);
    void pushToActiveSceneAfterThisUpdate(Scene* scene);
	
	void setTimerInterval(int interval);
	void setTimerMsg(unsigned int msg);
	void setCallTimerImmediate(bool bFlag);
	void setTimerRepeate(bool bRepeate);
	void setTimerCallbackCount(unsigned int count);
	void enableTimer();
	void disableTimer();
	void clearTimerVars();
	
	unsigned int getSceneWidth()	{	return m_cScreenWidth;	}
	unsigned int getSceneHeight()	{	return m_cScreenHeight;	}
	
    bool isPaused() {   return     m_bPause;    }
    
private:
	bxLinkedList<Scene*> m_cszSceneList;
	Scene* m_pActiveScene;                          //must not delete this pointer
  	Scene* m_pActiveSceneAfterThisUpdate;           //must not delete this pointer
    Scene* m_pPushMeToActiveSceneAfterThisUpdate;   //must not delete this pointer
	int m_cTimerInterval;
	int m_cElapsedTimer;
	unsigned int m_cTimerMsg;
	bool m_bCallTimerImmediate;
	bool m_bTimer;
	bool m_bTimerRepeate;
	unsigned int m_cScreenWidth;
	unsigned int m_cScreenHeight;
	unsigned int m_nTimerCallbackCount;
    
    unsigned long m_uElapsedTimeAfterPreviousFixedUpdate;
    bool m_bPause;
};

#endif