#include "rendererGL10.h"

#ifdef WIN32
rendererGL10::rendererGL10(HWND hWnd):rendererBase(hWnd, rendererBase::gl_fixed_pipeline)
#else
rendererGL10::rendererGL10():rendererBase(rendererBase::gl_fixed_pipeline)
#endif
{
}

rendererGL10::~rendererGL10()
{
}
