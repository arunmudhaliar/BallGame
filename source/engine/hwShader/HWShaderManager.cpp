#include "HWShaderManager.h"

//rendererBase::ERENDERER rendererBase::g_eRenderingTechnique=gl_fixed_pipeline;

HWShaderManager* HWShaderManager::g_pHWShaderManagerPtr=NULL;

HWShaderManager::HWShaderManager()
{
    g_pHWShaderManagerPtr=this;
    m_pDirectionalLightPtr=NULL;
}

HWShaderManager::~HWShaderManager()
{
	Reset();
}

void HWShaderManager::Init()
{
//	GLint nVarying_floats;
//	glGetIntegerv(GL_MAX_VARYING_FLOATS, &nVarying_floats);
//	GLint nVertexAttribs;
//	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nVertexAttribs);
//	GLint nVertexUnifrormComp;
//	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &nVertexUnifrormComp);
//	GLint nFragUnifrormComp;
//	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &nFragUnifrormComp);
//	GLint nVertexTexUnits;
//	glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &nVertexTexUnits);
//	GLint nFragTexUnits;
//	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &nFragTexUnits);
//
///*
//Maximum number of vertex attributes								GL_MAX_VERTEX_ATTRIBS
//Maximum number of uniform vertex vectors						GL_MAX_VERTEX_UNIFORM_VECTORS
//Maximum number of uniform fragment vectors						GL_MAX_FRAGMENT_UNIFORM_VECTORS
//Maximum number of varying vectors								GL_MAX_VARYING_VECTORS
//Maximum number of texture units usable in a vertex shader		GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS
//Maximum number of texture units usable in a fragment shader		GL_MAX_TEXTURE_IMAGE_UNITS
//*/
//	
//	DEBUG_PRINT("GL_MAX_VARYING_FLOATS = %d", nVarying_floats);
//	DEBUG_PRINT("GL_MAX_VERTEX_ATTRIBS = %d", nVertexAttribs);
//	DEBUG_PRINT("GL_MAX_VERTEX_UNIFORM_COMPONENTS = %d", nVertexUnifrormComp);
//	DEBUG_PRINT("GL_MAX_FRAGMENT_UNIFORM_COMPONENTS = %d", nFragUnifrormComp);
//	DEBUG_PRINT("(vs)GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS = %d", nVertexTexUnits);
//	DEBUG_PRINT("(fs)GL_MAX_TEXTURE_IMAGE_UNITS = %d", nVertexTexUnits);

    if(rendererBase::g_eRenderingTechnique==rendererBase::gl_programmable_pipeline)
    {
        LoadDefaultShaders();
    }
}

void HWShaderManager::Reset()
{
#if defined (USE_ProgrammablePipeLine)
	m_cvHWShaderLst.clearAll();
#endif
    m_pDirectionalLightPtr=NULL;
}

void HWShaderManager::LoadDefaultShaders()
{
#if defined (USE_ProgrammablePipeLine)
//	if(m_cDirectionalLightingShader.loadShader("shaders/ppDirectionalLighting.vsh", "shaders/ppDirectionalLighting.fsh"))
//		m_cvHWShaderLst.insertTail(&m_cDirectionalLightingShader);
    
    if(m_cDiffuseMapUnlit.loadShader("shaders/diffusemapunlit.vsh", "shaders/diffusemapunlit.fsh"))
		m_cvHWShaderLst.insertTail(&m_cDiffuseMapUnlit);

    if(m_cDiffuseMapLightMap.loadShader("shaders/diffusemap_lightmap.vsh", "shaders/diffusemap_lightmap.fsh"))
		m_cvHWShaderLst.insertTail(&m_cDiffuseMapLightMap);

    if(m_cGUIShader.loadShader("shaders/guishader.vsh", "shaders/guishader.fsh"))
		m_cvHWShaderLst.insertTail(&m_cGUIShader);
    
    if(m_cBlurShader.loadShader("shaders/blurshader.vsh", "shaders/blurshader.fsh"))
		m_cvHWShaderLst.insertTail(&m_cBlurShader);

#endif
}

#if defined (USE_ProgrammablePipeLine)
gxHWShader* HWShaderManager::GetHWShader(int index)
{
	if(index>=m_cvHWShaderLst.getSize()) return NULL;
	return m_cvHWShaderLst[index];
}
#endif

void HWShaderManager::update(float dt)
{
#if defined (USE_ProgrammablePipeLine)
    stLinkNode<gxHWShader*>* hwshaderNode=m_cvHWShaderLst.getHead();
	while(hwshaderNode)
	{
		gxHWShader* hwshader=hwshaderNode->getData();
		hwshader->updateShaderVars(dt);
		hwshaderNode=hwshaderNode->getNext();
	}
#endif
}
