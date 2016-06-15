#ifndef RENDERER_H
#define RENDERER_H

#include "../core/glincludes.h"
#include "../core/matrix4x4f.h"

class rendererBase
{
public:

	enum ERENDERER
	{
		gl_fixed_pipeline,
		gl_programmable_pipeline
	};

#ifdef WIN32
	rendererBase(HWND hWnd, ERENDERER technique);
#else
	rendererBase(ERENDERER technique);
#endif
	virtual ~rendererBase();

	bool setupRenderer();
	void destroyRenderer();

	void setViewPort(int x, int y, int cx, int cy);
	void loadDefaultRenderState();

	ERENDERER getRenderingTechnique()	{	return g_eRenderingTechnique;	}

	void swapGLBuffer();

	void setProjectionMatrixToGL(matrix4x4f* matrix);
	void setViewMatrixToGL(matrix4x4f* matrix);
	void setViewProjectionMatrix(matrix4x4f* matrix);

	const vector2f& getViewPortSz()	{	return m_cViewPortSz;	}

	matrix4x4f* getProjectionMatrix()		{	return m_pProjectionMatrixPtr;	}
	matrix4x4f* getViewMatrix()				{	return m_pViewMatrixPtr;	}
	matrix4x4f* getViewProjectionMatrix()	{	return m_pViewProjectionMatrixPtr;	}
	matrix4x4f* getOrthoProjectionMatrix()	{	return &m_cOrthogonalProjectionMatrix;	}

	void enableDirectionalLighting()		{	m_bDirectionalLighting=true;	}
	void disableDirectionalLighting()		{	m_bDirectionalLighting=false;	}

private:

#ifdef WIN32
	bool killGL();


	unsigned int	m_iPixelFormat;					// Holds The Results After Searching For A Match.
	HDC				m_hDC;							// Private GDI Device Context.
	HGLRC			m_hRC;							// Permanent Rendering Context.
	HWND			m_hWnd;							// Holds Our Window Handle.
#endif

	vector2f	m_cViewPortSz;
	matrix4x4f* m_pProjectionMatrixPtr;			//must not delete this pointer
	matrix4x4f* m_pViewMatrixPtr;				//must not delete this pointer
	matrix4x4f* m_pViewProjectionMatrixPtr;     //must not delete this pointer
    matrix4x4f  m_cOrthogonalProjectionMatrix;
	bool		m_bDirectionalLighting;
public:
	static ERENDERER g_eRenderingTechnique;
    static unsigned int g_nTrisRendered;
};
#endif