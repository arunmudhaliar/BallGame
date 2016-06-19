#pragma once

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