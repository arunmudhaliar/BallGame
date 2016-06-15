#ifndef RENDERERGL10_H
#define RENDERERGL10_H

#include "renderer.h"

class rendererGL10 : public rendererBase
{
public:
#ifdef WIN32
	rendererGL10(HWND hWnd);
#else
	rendererGL10();
#endif
	~rendererGL10();
};
#endif