#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef unsigned char	byte;

#define GX_DELETE(x)		if(x){delete x; x=NULL;}
#define GX_DELETE_ARY(x)	if(x){delete [] x; x=NULL;}

#define GX_SWAP_INT(x1, x2)     { int t=x1; x1=x2; x2=t;    }
#define GX_SWAP_FLOAT(x1, x2)   { float t=x1; x1=x2; x2=t;    }

#define GX_MAX_INT  2147483647
#define GX_MIN_INT (-2147483647 - 1)

#if defined(TARGET_IPHONE_SIMULATOR) || defined(TARGET_OS_IPHONE)
	#define USE_OPENAL	1
#elif defined(ANDROID)
	#define USE_OPENSL	1
    //#define USE_OPENAL	1
#endif

#if !defined(WIN32)
#define USE_ProgrammablePipeLine
#endif

extern "C" {
    void setResourcePath(const char* path);
    void setCommonResourcePath(const char* path);
    const char* getResourcePath(const char* relativepath);
    const char* getCommonResourcePath(const char* relativepath);
    const char* appendPath(const char* path1, const char* path2);
}

#endif
