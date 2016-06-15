#ifndef HWSHADERMANAGER_H
#define HWSHADERMANAGER_H

#include "../renderer/renderer.h"
#include "gxShader.h"
#include "../core/gxLight.h"

#if defined (USE_ProgrammablePipeLine)
#include "gxDirectionalLightingShader.h"
#include "gxWhiteShader.h"

#include "gxDiffuseMapUnlit.h"
#include "gxDiffuseMapLightMap.h"

#include "gxGUIShader.h"

#include "gxBlurShader.h"

#include "../util/bxLinkedList.h"
#endif

class HWShaderManager
{
public:
	HWShaderManager();
	~HWShaderManager();
	
	void Init();
	void Reset();
    
    void update(float dt);
	
#if defined (USE_ProgrammablePipeLine)
	gxHWShader* GetHWShader(int index);
#endif
	
    void setDirectionalLightPtr(gxLight* ptr)   {   m_pDirectionalLightPtr=ptr;     }
    const gxLight* getDirectionalLightPtr()     {   return m_pDirectionalLightPtr;  }
    
private:
	void LoadDefaultShaders();
	
#if defined (USE_ProgrammablePipeLine)
	//default shaders
	gxDirectionalLightingShader m_cDirectionalLightingShader;
	gxWhiteShader m_cWhiteShader;
    gxDiffuseMapUnlit m_cDiffuseMapUnlit;
    gxDiffuseMapLightMap m_cDiffuseMapLightMap;
    
    gxGUIShader m_cGUIShader;
    
    gxBlurShader m_cBlurShader;
    
	bxLinkedList<gxHWShader*> m_cvHWShaderLst;
#endif
    
    gxLight* m_pDirectionalLightPtr;
public:
    static HWShaderManager* g_pHWShaderManagerPtr;
    static HWShaderManager* getHWShaderManager()    {   return g_pHWShaderManagerPtr;   }
};

#endif
