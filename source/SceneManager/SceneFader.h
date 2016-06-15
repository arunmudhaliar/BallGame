
#ifndef SCENEFADER_H
#define SCENEFADER_H

#include "CommonData.h"

class SceneFader
{
public:
	void initFader(CommonData* pCommonData);
	void resetFader();
	void setFaderRGB(float r, float g, float b);
	void setFaderAlpha(float min, float max);
	void setFaderSpeed(float speed);
	void updateFader(float dt);
	void renderFader();
	
	virtual void onFaderComplete(unsigned int msg);
	
	void startFader(unsigned int msg, bool bHold=false)	{	m_bRunning=true; m_cMsg=msg; m_bHoldOnComplete=bHold;   }
	bool isFaderComplete()		{	return !m_bRunning;	}

    void releaseFaderOnHold()   {   m_bHoldOnComplete=false;    }
    
private:
	unsigned int m_cMsg;
	float m_cszColor[4];
	float m_cMaxAlpha;
	float m_cMinAlpha;
	bool m_bRunning;
    bool m_bHoldOnComplete;
	float m_cSpeed;
	CommonData* m_pCommonData;
};

#endif