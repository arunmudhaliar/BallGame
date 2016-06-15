#include "gxDebug.h"
#include <assert.h>

#include "../core/glincludes.h"
#include "../core/types.h"

#if defined(WIN32) //&& defined(_DEBUG)
void DEBUG_PRINT(const char* format, ...)
{
	char buffer[1024];
	va_list v;
	va_start(v,format);
	int ret = vsprintf(buffer, format, v);
	OutputDebugString(buffer);
	OutputDebugString("\n");
	va_end(v);
}

#elif defined(TARGET_IPHONE_SIMULATOR) || defined(TARGET_OS_IPHONE)
void DEBUG_PRINT(const char* format, ...)
								{
									char buffer[1024];
									va_list v;
									va_start(v,format);
									int ret = vsprintf(buffer, format, v);
									NSLog(@"%s", buffer);
									va_end(v);
								}

#endif


void DEBUG_ASSERT(bool condition, const char* string)
{
	if(!condition)
		DEBUG_PRINT("%s", string);
	assert(condition);
}

#if !defined(WIN32)
void DEBUG_DRAW_LINE(int x1, int y1, int x2, int y2)
{
	int cszTileBuffer[]={x1, y1, x2, y2};
#if defined (USE_ProgrammablePipeLine)
#else
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FIXED, 0, cszTileBuffer);
	glDrawArrays(GL_LINES, 0, 2);
	glDisableClientState(GL_VERTEX_ARRAY);	
#endif
}

void DEBUG_DRAW_RECT(int x, int y, int cx, int cy)
{
    int cszTileBuffer[]={
                            x,      y,
                            x,      y+cy,
                            x+cx,   y+cy,
                            x+cx,   y
                        };
#if defined (USE_ProgrammablePipeLine)
#else
 	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FIXED, 0, cszTileBuffer);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
#endif
}
#endif

void DEBUG_DRAW_RECTf(float x, float y, float cx, float cy)
{
    float cszTileBuffer[]={
        x,      y,
        x,      y+cy,
        x+cx,   y+cy,
        x+cx,   y
    };
    
#if defined (USE_ProgrammablePipeLine)
#else
 	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, cszTileBuffer);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
#endif
}

#if !defined(WIN32)
void DEBUG_DRAW_FILL_RECT(int x, int y, int cx, int cy)
{
    int cszTileBuffer[]={
        x,      y,
        x,      y+cy,
        x+cx,   y+cy,
        x+cx,   y
    };
    
#if defined (USE_ProgrammablePipeLine)
#else
 	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FIXED, 0, cszTileBuffer);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
#endif
}
#endif

void DEBUG_DRAW_FILL_RECTf(float x, float y, float cx, float cy)
{
    float cszTileBuffer[]={
        x,      y,
        x,      y+cy,
        x+cx,   y+cy,
        x+cx,   y
    };
    
#if defined (USE_ProgrammablePipeLine)
#else
 	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, cszTileBuffer);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
#endif
}

void DEBUG_DRAW_FILL_RECT_ALPA(float x, float y, float cx, float cy)
{
    float cszTileBuffer[]={
        x,      y,
        x,      y+cy,
        x+cx,   y+cy,
        x+cx,   y
    };
    
#if defined (USE_ProgrammablePipeLine)
#else
    glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, cszTileBuffer);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   // glColor4f(r, g , b , alpha);
    
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
    glDisable(GL_BLEND);
#endif
}


