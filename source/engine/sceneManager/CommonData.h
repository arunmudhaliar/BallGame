#pragma once

#ifdef WIN32
#include <Windows.h>
#endif

#include "../core/types.h"
#include "../core/FontManager.h"
#include <string.h>
#include "../util/gxDebug.h"
#include "../core/TextureManager.h"

#include "../renderer/rendererGL10.h"
#include "../core/object3d.h"
#include "../../game/playerData.h"

class CommonData
{
public:
	CommonData()
	{
        m_iScreenWidth=m_iScreenHeight=1;
        m_pArialBld15Ptr=NULL;
		m_pRendererPtr=NULL;
	}
	
	~CommonData()
	{
	}
	
	void createRenderer()
	{
		m_pRendererPtr = new rendererGL10(m_hWnd);
		if(!m_pRendererPtr->setupRenderer())
			return;
		m_pRendererPtr->loadDefaultRenderState();
		objectBase::g_pRendererPtr=m_pRendererPtr;
	}

private:
    void reset()
    {
		m_pRendererPtr=NULL;
        m_cFontManager.reset(false);
		GX_DELETE(m_pRendererPtr);
    }
public:
    
    void init(const char* androidResourceDirectory, bool bLoadHDResource, rendererBase::ERENDERER technique, HWND hWnd)
	{
		m_hWnd=hWnd;
        m_eRenderingTechnique=technique;
        
		createRenderer();

        setResourcePath("./res/win32/");
        setCommonResourcePath("./res/Common/");
        m_cFontManager.init(technique);
        loadFonts(false);
    }
    
    void loadFonts(bool reload)
    {
        m_cFontManager.reset(reload);
        m_pArialBld15Ptr=m_cFontManager.loadFont(getResourcePath("fonts/arial_bold15.ecf"));
    }
    

	void setHWND(HWND hWnd)			{	m_hWnd=hWnd;				}
	HWND getHWND()					{	return m_hWnd;				}

    void setScreenSize(int cx, int cy)  {   m_iScreenWidth=cx; m_iScreenHeight=cy;  }
    
    int getScreenWidth()    {   return m_iScreenWidth;      }
    int getScreenHeight()   {   return m_iScreenHeight;     }
    
    Font* getArialBold15Font()      {   return m_pArialBld15Ptr;        }
    
    void shutDown()
    {
        //do the necessary cleanups here
        reset();
    }
    
    void setDontClearGLBuffers(bool flag)
    {
        m_bDontClearGLBuffer=flag;
    }
    
    bool isDontClearGLBuffers()
    {
        return m_bDontClearGLBuffer;
    }
    
    rendererBase::ERENDERER getRenderingTechnique() {   return m_eRenderingTechnique;   }

	void setRendererPtr(rendererBase* ptr)	{	m_pRendererPtr=ptr;		}
	rendererBase* getRendererPtr()			{	return m_pRendererPtr;	}

	playerData* getPlayerData()		{	return &m_cPlayerData;	}

private:
    int m_iScreenWidth;
    int m_iScreenHeight;

    FontManager m_cFontManager;
    Font* m_pArialBld15Ptr;

    bool m_bDontClearGLBuffer;
    rendererBase::ERENDERER m_eRenderingTechnique;
	rendererBase* m_pRendererPtr;
	HWND m_hWnd;
	playerData m_cPlayerData;
};