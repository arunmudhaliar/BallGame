#ifndef OBC_UTIL_H
#define OBC_UTIL_H

#include "../core/glincludes.h"

#include "gxDebug.h"
#include "../zlib-1.2.4/zlib.h"
#include <string.h>
#include <errno.h>

class objC_util
{
public:
	static const char* getPath(const char* path)
	{
		return path;
	}

    static unsigned int loadBuffer(unsigned char* buffer, bool bAlpha, unsigned int width, unsigned int height, unsigned int bpp)
    {
        unsigned int texID=0;
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
        
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        switch (bpp) {
            case 16:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, buffer);
                break;
            case 32:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
                break;
            default:
                break;
        }
        
        
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glDisable(GL_TEXTURE_2D);

        return texID;
    }
};
#endif
