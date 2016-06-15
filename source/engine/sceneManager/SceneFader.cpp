
#include "SceneFader.h"
#include "../core/glincludes.h"
#include "../util/gxDebug.h"
#include "../renderer/renderer.h"

void SceneFader::initFader(CommonData* pCommonData)
{
	m_pCommonData=pCommonData;
	resetFader();
}

void SceneFader::resetFader()
{
	memset(m_cszColor, 0, sizeof(float)*4);
	m_cMaxAlpha=0.0f;
	m_cMinAlpha=0.0f;
	m_bRunning=false;
	m_cMsg=0;
	m_cSpeed=0.5f;
    releaseFaderOnHold();
}

void SceneFader::setFaderRGB(float r, float g, float b)
{
	m_cszColor[0]=r;
	m_cszColor[1]=g;
	m_cszColor[2]=b;
	m_cszColor[3]=1.0f;
}

void SceneFader::setFaderAlpha(float min, float max)
{
	m_cMinAlpha=min;
	m_cMaxAlpha=max;
	
	//if(m_cMaxAlpha>m_cMinAlpha)
		m_cszColor[3]=min;
	//else
	//	m_cszColor[3]=max;
}

void SceneFader::setFaderSpeed(float speed)
{
	m_cSpeed=speed;
}

void SceneFader::updateFader(float dt)
{
	if(!m_bRunning) return;

	if(m_cMaxAlpha>m_cMinAlpha)
	{
		m_cszColor[3]+=(dt*m_cSpeed);
		if(m_cszColor[3]>=m_cMaxAlpha)
		{
			m_cszColor[3]=m_cMaxAlpha;
			m_bRunning=false;
			onFaderComplete(m_cMsg);
			m_cMsg=0;
		}
	}
	else
	{
		m_cszColor[3]-=(dt*m_cSpeed);
		if(m_cszColor[3]<=m_cMaxAlpha)
		{
			m_cszColor[3]=m_cMaxAlpha;
			m_bRunning=false;
			onFaderComplete(m_cMsg);
			m_cMsg=0;
		}
	}
	
	//DEBUG_PRINT("alpha=%f", m_cszColor[3]);
}

void SceneFader::renderFader()
{
    if(rendererBase::g_eRenderingTechnique==rendererBase::gl_programmable_pipeline) return;

	if(!m_bRunning && !m_bHoldOnComplete) return;
	    
	const short vertLst[8] =
	{
		(0),		(0),		
		(0),		(m_pCommonData->getScreenHeight()),	
		(0+m_pCommonData->getScreenWidth()),	(0),		
		(0+m_pCommonData->getScreenWidth()),	(m_pCommonData->getScreenHeight()),
	};
	

	//m_pCommonData->getRenderer()->Begin2DDraw();
	
   
    glEnableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_TEXTURE_2D);
    glVertexPointer(2, GL_SHORT, 0, vertLst);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(m_cszColor[0], m_cszColor[1], m_cszColor[2], m_cszColor[3]);
    glPushMatrix();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glPopMatrix();
	glDisable(GL_BLEND);
	glDisableClientState(GL_VERTEX_ARRAY);
    
//	float r=m_cszColor[0];
//	float g=m_cszColor[1];
//	float b=m_cszColor[2];
//	float a=m_cszColor[3];

	//m_pCommonData->getRenderer()->End2DDraw();
}

void SceneFader::onFaderComplete(unsigned int msg)
{

}

