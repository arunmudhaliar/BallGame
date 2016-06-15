#include "renderer.h"
#include "..\core\rect.h"


rendererBase::ERENDERER rendererBase::g_eRenderingTechnique=gl_fixed_pipeline;
unsigned int rendererBase::g_nTrisRendered=0;

rendererBase::rendererBase(HWND hWnd, ERENDERER technique):
	m_hWnd(hWnd),
	m_iPixelFormat(0),
	m_hDC(0),
	m_hRC(0)
{
	g_eRenderingTechnique=technique;
	m_pProjectionMatrixPtr=NULL;
	m_pViewMatrixPtr=NULL;
	m_pViewProjectionMatrixPtr=NULL;
	m_cViewPortSz.set(1.0f, 1.0f);
}
	
rendererBase::~rendererBase()
{
	destroyRenderer();
}

bool rendererBase::setupRenderer()
{
#ifdef WIN32
	static	PIXELFORMATDESCRIPTOR pfd=							// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),							// Size Of This Pixel Format Descriptor
		1,														// Version Number
		PFD_DRAW_TO_WINDOW |									// Format Must Support Window
		PFD_SUPPORT_OPENGL |									// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,										// Must Support Double Buffering
		PFD_TYPE_RGBA,											// Request An RGBA Format
		24,														// Select Our Color Depth
		0, 0, 0, 0, 0, 0,										// Color Bits Ignored
		0,														// No Alpha Buffer
		0,														// Shift Bit Ignored
		0,														// No Accumulation Buffer
		0, 0, 0, 0,												// Accumulation Bits Ignored
		24,														// 16Bit Z-Buffer (Depth Buffer)  
		0,														// No Stencil Buffer
		0,														// No Auxiliary Buffer
		PFD_MAIN_PLANE,											// Main Drawing Layer
		0,														// Reserved
		0, 0, 0													// Layer Masks Ignored
	};
	
	if (!(m_hDC=GetDC(m_hWnd)))										// Did We Get A Device Context?
	{
		destroyRenderer();												// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;											// Return GX_FALSE
	}
	
	if (!(m_iPixelFormat=ChoosePixelFormat(m_hDC,&pfd)))				// Did Windows Find A Matching Pixel Format?
	{
		destroyRenderer();												// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;											// Return GX_FALSE
	}
	
	if(!SetPixelFormat(m_hDC,m_iPixelFormat,&pfd))					// Are We Able To Set The Pixel Format?
	{
		destroyRenderer();												// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;											// Return GX_FALSE
	}
	
	if (!(m_hRC=wglCreateContext(m_hDC)))							// Are We Able To Get A Rendering Context?
	{
		destroyRenderer();												// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;											// Return GX_FALSE
	}	
	
	if(!wglMakeCurrent(m_hDC,m_hRC))								// Try To Activate The Rendering Context
	{
		destroyRenderer();												// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;											// Return GX_FALSE
	}
	
	glewInit();
	//if (!GLEW_VERSION_2_0)
	//{
	//	MessageBox(NULL, "OpenGL 2.0 not supported. Shaders won't work !!!", "ERROR", MB_OK|MB_ICONEXCLAMATION);
	//}
#endif

	return true;
}

void rendererBase::destroyRenderer()
{
#ifdef WIN32
	killGL();
#endif
}

#ifdef WIN32
bool rendererBase::killGL()
{

	bool flag=false;
	
	if(m_hRC)													// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))						// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			flag=true;
		}
		
		if (!wglDeleteContext(m_hRC))							// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			flag=true;
		}
		m_hRC=NULL;											// Set RC To NULL
	}
	
	return flag;
}
#endif

void rendererBase::setViewPort(int x, int y, int cx, int cy)
{
	bxRectf viewportRect(x, y, cx, cy);
	vector2f centerAlignedPos(viewportRect.m_pos - viewportRect.m_size*0.5f);
	/*m_cOrthogonalProjectionMatrix.setOrtho(
		centerAlignedPos.x,
		centerAlignedPos.x + viewportRect.m_size.x,
		centerAlignedPos.y + viewportRect.m_size.y,
		centerAlignedPos.y,
		-100.0f, 1000.0f);*/
	//viewPortRectangle.set(x, y, cx, cy);
	//CHECK_GL_ERROR(glViewport((int)x, (int)y, (int)cx, (int)cy));

    m_cOrthogonalProjectionMatrix.setOrtho(0.0f, (float)cx, (float)cy, 0.0f, 0.0f, 10.0f);
	m_cViewPortSz.set((float)cx, (float)cy);
	glViewport(x, y, cx, cy);
}

void rendererBase::loadDefaultRenderState()
{
	glClearColor(0.0f, 0.0f, 0.1f, 1.0f);				// Black Background
	//glClearColor(0.5f, 0.5f, 0.5f, 1.0f);				// Black Background
	//glShadeModel(GL_FLAT);							// Enable Smooth Shading
	//glClearDepth(1.0f);									// Depth Buffer Setup
	
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);								// Enable culling
	
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do (GL_LEQUAL is must for shadow fonts)
	glDisable(GL_DEPTH_TEST);							// Enables Depth Testing
}

void rendererBase::swapGLBuffer()
{
	SwapBuffers(m_hDC);
}

void rendererBase::setProjectionMatrixToGL(matrix4x4f* matrix)
{
	m_pProjectionMatrixPtr=matrix;

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(matrix->getMatrix());
    glMatrixMode(GL_MODELVIEW);
}

void rendererBase::setViewMatrixToGL(matrix4x4f* matrix)
{
	m_pViewMatrixPtr=matrix;
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(matrix->getMatrix());
}

void rendererBase::setViewProjectionMatrix(matrix4x4f* matrix)
{
	m_pViewProjectionMatrixPtr=matrix;
}