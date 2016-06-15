#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include "../util/bxLinkedList.h"
#include "glincludes.h"
#include "../util/gxFile.h"
#include "../renderer/renderer.h"

#define DEBUG_KERNING   0.0f

class Font
{
public:
    enum ALIGNMENT
    {
        ALIGN_LEFT,
        ALIGN_RIGHT,
        ALIGN_CENTER,
        ALIGN_JUSTIFY
    };
    

    Font();
    ~Font();

    void reset();
    bool load(gxFile& file);
    void draw();
    
    bool drawString(const char* str, int x, int y, bool bCentered=false, bool bShadowed=false, float elapsetime=0.0f);
    
    unsigned int getTexID() const {   return m_iTexID;    }
    unsigned short getLineHeight() const {   return m_iLineHeight;   }
    void setLineHeight(unsigned short height)
    {
        m_iLineHeight=height;
    }
    
    void setDeleteGLTexture(bool flag)  {   m_bDeleteGLTexture=flag;    }
    void setRGBA(float r, float g, float b, float a=1.0f)
    {
        m_cszRGBA[0]=r;        m_cszRGBA[1]=g;        m_cszRGBA[2]=b;        m_cszRGBA[3]=a;        
    }
    
    void setYOffset(float offy) {   m_fYOffset=offy;    }
    
private:    
    float calculateStringWidthInPixelTillNewLine(const char* str, int nChar, int iCurIndex);
    
    int m_iBitmapWidth;
    int m_iBitmapHeight;
    int m_iBaseChar;
    int m_nChars;
    short m_iSpaceWidth;
    unsigned short m_iLineHeight;
    
    short* m_pszCharOffsetX;
    short* m_pszCharOffsetY;
    unsigned short* m_pszCharW;
    unsigned short* m_pszCharH;
    short* m_pszCharD;
    float* m_pszU;
    float* m_pszV;
    float* m_pszU_width;
    float* m_pszU_height;
    
    unsigned int m_iTexID;
    
    short m_cszVertCoordList[12*256];
    float m_cszTexCoordList[12*256];
    bool m_bDeleteGLTexture;
    float m_cszRGBA[4];
    float m_fYOffset;   //for small adjustments
};

class FontManager
{
public:
    FontManager();
    ~FontManager();
    
    void init(rendererBase::ERENDERER technique);
    void reset(bool reload);
    Font* loadFont(const char* filename);
    Font* getFont(int index)                {   return m_cvFontList[index]; }

private:
    bxLinkedList<Font*> m_cvFontList;
};

#endif
