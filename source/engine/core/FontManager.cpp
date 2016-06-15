
#include "FontManager.h"
#include "../util/objC_util.h"

Font::Font()
{

    m_iBitmapWidth=m_iBitmapHeight=m_iBaseChar=m_nChars=0;
    m_pszU=m_pszV=m_pszU_width=m_pszU_height=NULL;
    m_pszCharW=m_pszCharH=NULL;
    m_pszCharD=NULL;
    m_pszCharOffsetX=m_pszCharOffsetY=NULL;
    m_iTexID=0;
    m_iSpaceWidth=0;
    m_iLineHeight=0;
    m_bDeleteGLTexture=true;
    setRGBA(1.0f, 1.0f, 1.0f, 1.0f);
    setYOffset(0.0f);
}

Font::~Font()
{
    reset();
}

void Font::reset()
{
    if(m_iTexID>0 && m_bDeleteGLTexture)
    {
        glDeleteTextures(1, &m_iTexID);
        m_iTexID=0;
    }
    
    GX_DELETE_ARY(m_pszCharOffsetX);
    GX_DELETE_ARY(m_pszCharOffsetY);
    GX_DELETE_ARY(m_pszCharW);
    GX_DELETE_ARY(m_pszCharH);
    GX_DELETE_ARY(m_pszCharD);
    
    GX_DELETE_ARY(m_pszU);
    GX_DELETE_ARY(m_pszV);
    GX_DELETE_ARY(m_pszU_width);
    GX_DELETE_ARY(m_pszU_height);
}

bool Font::load(gxFile& file)
{
    file.Read(m_iBitmapWidth);
    file.Read(m_iBitmapHeight);
    file.Read(m_nChars);
    file.Read(m_iBaseChar);
    file.Read(m_iSpaceWidth);    
    
    if(m_nChars==0) return false;
    
    unsigned char* buffer=new unsigned char[m_iBitmapHeight*m_iBitmapWidth*2];
    
    m_pszU=new float[m_nChars];
    m_pszV=new float[m_nChars];
    m_pszU_width=new float[m_nChars];
    m_pszU_height=new float[m_nChars];
    m_pszCharW=new unsigned short[m_nChars];
    m_pszCharH=new unsigned short[m_nChars];
    m_pszCharD=new short[m_nChars];
    m_pszCharOffsetX=new short[m_nChars];
    m_pszCharOffsetY=new short[m_nChars];
    
    for(int x=0;x<m_nChars;x++)
    {
        file.Read(m_pszCharOffsetX[x]);
        file.Read(m_pszCharOffsetY[x]);

        file.Read(m_pszCharW[x]);
        file.Read(m_pszCharH[x]);
        file.Read(m_pszCharD[x]);
        file.Read(m_pszU[x]);
        file.Read(m_pszV[x]);
        file.Read(m_pszU_width[x]);
        file.Read(m_pszU_height[x]);
    }
    
    file.Read(m_iLineHeight);
    file.ReadBuffer(buffer, m_iBitmapHeight*m_iBitmapWidth*2);
    m_iTexID=objC_util::loadBuffer(buffer, true, m_iBitmapWidth, m_iBitmapHeight, 16);
    GX_DELETE_ARY(buffer);
    
    return true;
}

void Font::draw()
{
    const int vertLst[8] =
	{
		m_iBitmapWidth*65536,	0,
		0,                      0,
		m_iBitmapWidth*65536,	m_iBitmapHeight*65536,
		0,                      m_iBitmapHeight*65536
	};
    
    const int tex[8] =
	{
		65536,  0,
		0,      0,
		65536,  65536,
		0,      65536,
	};

#if defined (USE_ProgrammablePipeLine)
#else
#if ARUN_COMMENTED
    glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FIXED, 0, vertLst);

    glActiveTexture(GL_TEXTURE0);
    glClientActiveTexture(GL_TEXTURE0);
    glTexCoordPointer(2, GL_FIXED, 0, tex);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_iTexID);
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    //arun_commenting_alphatest glEnable(GL_ALPHA_TEST);
    //arun_commenting_alphatest glAlphaFuncx(GL_GREATER, 6553); //0.1f
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisable(GL_BLEND);
    //arun_commenting_alphatest glDisable(GL_ALPHA_TEST);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glActiveTexture(GL_TEXTURE0);
    glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_VERTEX_ARRAY);
#endif
#endif
}

float Font::calculateStringWidthInPixelTillNewLine(const char* str, int nChar, int iCurIndex)
{
    float kerning=DEBUG_KERNING;
    float width=0.0f;
    int diff=nChar-iCurIndex;
    int cntr=0;
    while(diff--)
    {
        int actual_char=(unsigned char)str[iCurIndex+cntr];
        int ch=(unsigned char)str[iCurIndex+cntr]-m_iBaseChar;

        if(actual_char=='\n')
        {
            break;
        }
        else if(actual_char=='\t')
        {
            width+=(m_pszCharW[32-m_iBaseChar]*5);
            continue;
        }
        if(ch<0 || ch>=m_nChars)
        {
            cntr++;
            continue;
        }
        width+=((float)m_pszCharD[ch]+kerning);
        cntr++;
    }
    
    return width;
}

#define CHAR_DELAY  0.05f
bool Font::drawString(const char* str, int x, int y, bool bCentered, bool bShadowed, float elapsetime)
{
    bool retVal=true;
    int actual_len=strlen(str);
    int len=(elapsetime==0.0f)?actual_len:(int)(elapsetime/CHAR_DELAY);
    if(len>actual_len){len=actual_len;}
    else
    { retVal=false; }
    
    int cntr=0;
    
    float raster_pos_x=0.0f;
    float raster_pos_y=0.0f;
    float kerning=DEBUG_KERNING;

    float width=0.0f;
    if(bCentered)
    {
        width=calculateStringWidthInPixelTillNewLine(str, len, 0);
        raster_pos_x=-width*0.5f;
    }
    
    for(int xx=0;xx<len;xx++)
    {
        int actual_char=(unsigned char)str[xx];
        int ch=(unsigned char)str[xx]-m_iBaseChar;
        
        if(actual_char=='\n')
        {
            raster_pos_y+=(float)m_iLineHeight;
            raster_pos_x=0.0f;
            if(bCentered)
            {
                width=calculateStringWidthInPixelTillNewLine(str, len, xx+1);
                raster_pos_x=-width*0.5f;
            }
            continue;
        }
        else if(actual_char=='\t')
        {
            raster_pos_x+=(m_pszCharW[32-m_iBaseChar]*5);
            continue;
        }
        
        if(ch<0 || ch>=m_nChars)
        {
            continue;
        }

        int offx=m_pszCharOffsetX[ch];
        int offy=m_pszCharOffsetY[ch];
        
        float left=raster_pos_x+offx;
        float right=raster_pos_x+m_pszCharW[ch]+offx;
        float top=raster_pos_y+offy;
        float bottom=raster_pos_y+m_pszCharH[ch]+offy;
        
        m_cszVertCoordList[cntr*12+0]=(short)right;     m_cszVertCoordList[cntr*12+1]=(short)top;
        m_cszVertCoordList[cntr*12+2]=(short)left;      m_cszVertCoordList[cntr*12+3]=(short)top;
        m_cszVertCoordList[cntr*12+4]=(short)right;     m_cszVertCoordList[cntr*12+5]=(short)bottom;
        m_cszVertCoordList[cntr*12+6]=(short)left;      m_cszVertCoordList[cntr*12+7]=(short)bottom;
        m_cszVertCoordList[cntr*12+8]=(short)right;     m_cszVertCoordList[cntr*12+9]=(short)bottom;
        m_cszVertCoordList[cntr*12+10]=(short)left;     m_cszVertCoordList[cntr*12+11]=(short)top;

        raster_pos_x+=((float)m_pszCharD[ch]+kerning);
        
        m_cszTexCoordList[cntr*12+0]=(m_pszU[ch]+m_pszU_width[ch]);   m_cszTexCoordList[cntr*12+1]=m_pszV[ch];
        m_cszTexCoordList[cntr*12+2]=m_pszU[ch];                      m_cszTexCoordList[cntr*12+3]=m_pszV[ch];
        m_cszTexCoordList[cntr*12+4]=(m_pszU[ch]+m_pszU_width[ch]);   m_cszTexCoordList[cntr*12+5]=(m_pszV[ch]+m_pszU_height[ch]);
        m_cszTexCoordList[cntr*12+6]=m_pszU[ch];                      m_cszTexCoordList[cntr*12+7]=(m_pszV[ch]+m_pszU_height[ch]);
        m_cszTexCoordList[cntr*12+8]=(m_pszU[ch]+m_pszU_width[ch]);   m_cszTexCoordList[cntr*12+9]=(m_pszV[ch]+m_pszU_height[ch]);
        m_cszTexCoordList[cntr*12+10]=m_pszU[ch];                     m_cszTexCoordList[cntr*12+11]=m_pszV[ch];

        cntr++;
    }
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_SHORT, 0, m_cszVertCoordList);
    
    glActiveTexture(GL_TEXTURE0);
    glClientActiveTexture(GL_TEXTURE0);
    glTexCoordPointer(2, GL_FLOAT, 0, m_cszTexCoordList);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_iTexID);
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();
    if(bShadowed)
    {   
        glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
        glTranslatef((float)(x+1), (float)(y+1)+m_fYOffset, 0.0f);
        glDrawArrays(GL_TRIANGLES, 0, cntr*6);
        glTranslatef(-1, -1, 0);
    }
    else
    {
		glTranslatef((float)x, (float)y+m_fYOffset, 0.0f);
    }
    glColor4f(m_cszRGBA[0], m_cszRGBA[1], m_cszRGBA[2], m_cszRGBA[3]);
    glDrawArrays(GL_TRIANGLES, 0, cntr*6);
    glPopMatrix();
    
    glDisable(GL_BLEND);    
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glActiveTexture(GL_TEXTURE0);
    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_VERTEX_ARRAY);
    
    return retVal;
}

FontManager::FontManager()
{

}

FontManager::~FontManager()
{
	reset(false);
}

void FontManager::init(rendererBase::ERENDERER technique)
{
}

void FontManager::reset(bool reload)
{
    for(int x=0;x<m_cvFontList.getSize();x++)
    {
        Font* font=m_cvFontList[x];
        if(reload)font->setDeleteGLTexture(!reload);
        GX_DELETE(font);
    }
    m_cvFontList.clearAll();
}

Font* FontManager::loadFont(const char* filename)
{
    Font* newFont=new Font();

    gxFile file;
    file.OpenFile(filename, gxFile::FILE_r);
    newFont->load(file);
    file.CloseFile();
    
    m_cvFontList.insertTail(newFont);
    
    return newFont;
}

