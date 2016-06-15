#include "TextureManager.h"
#include "../util/pngLoader.h"

CTextureManager::CTextureManager()
{
    m_iTotalTextureMemory=0;
}

CTextureManager::~CTextureManager()
{
	Reset();
}

stTexturePacket* CTextureManager::LoadTexture(const char* aFileName, const char* aOpFileName)
{
	if(aFileName && aOpFileName)	//if both textures are there
	{
		int i=0;
		for(i=0;i<(int)iTexturePacket.getSize();i++)
		{
			if(iTexturePacket[i]->iTextureName && (strcmp(aFileName, iTexturePacket[i]->iTextureName)==0))
			{
				if(iTexturePacket[i]->iOpTextureName && (strcmp(aOpFileName, iTexturePacket[i]->iOpTextureName)==0))
					break;
			}
		}
		if(i!=(int)iTexturePacket.getSize())	return iTexturePacket[i];	//if texture already exist in list
	}
	else 
	{
		if(aFileName)
		{
			int i=0;
			for(i=0;i<(int)iTexturePacket.getSize();i++)
			{
				if(iTexturePacket[i]->iTextureName && (strcmp(aFileName, iTexturePacket[i]->iTextureName)==0))
				{
					if(!iTexturePacket[i]->iOpTextureName)
						break;
				}
			}
			if(i!=(int)iTexturePacket.getSize())	return iTexturePacket[i];	//if texture already exist in list
		}//only diffuse
		else if(aOpFileName)
		{
			int i=0;
			for(i=0;i<(int)iTexturePacket.getSize();i++)
			{
				if(iTexturePacket[i]->iOpTextureName && (strcmp(aOpFileName, iTexturePacket[i]->iOpTextureName)==0))
				{
					if(!iTexturePacket[i]->iTextureName)
						break;
				}
			}
			if(i!=(int)iTexturePacket.getSize())	return iTexturePacket[i];	//if texture already exist in list
		}// only optex
	} // if there is only one tex

	stTexturePacket*	aNewTexturePacket	= new stTexturePacket();
	
	unsigned int texID=0;
	bool alpha_tex=false;

    //texture loading utility
    glEnable(GL_TEXTURE_2D);
	texID=read_png_file(aFileName, alpha_tex, aNewTexturePacket->m_cWidth, aNewTexturePacket->m_cHeight, aNewTexturePacket->m_cBpp);
    aNewTexturePacket->bAlphaTex=alpha_tex;
    glDisable(GL_TEXTURE_2D);

    if(texID>0)
    {
        m_iTotalTextureMemory+=(unsigned int)(aNewTexturePacket->m_cWidth*aNewTexturePacket->m_cHeight*aNewTexturePacket->m_cBpp);
    }

	if(aFileName)
	{
		aNewTexturePacket->iTextureName = new char[strlen(aFileName)+1];
		strcpy(aNewTexturePacket->iTextureName, aFileName);
		aNewTexturePacket->iTextureName[strlen(aFileName)]	= '\0';
	}

	if(aOpFileName)
	{
		aNewTexturePacket->iOpTextureName = new char[strlen(aOpFileName)+1];
		strcpy(aNewTexturePacket->iOpTextureName, aOpFileName);
		aNewTexturePacket->iOpTextureName[strlen(aOpFileName)]	= '\0';
	}

	aNewTexturePacket->bAlphaTex	= alpha_tex;
	aNewTexturePacket->iTextureID	= texID;

	iTexturePacket.insertTail(aNewTexturePacket);
	
	return aNewTexturePacket;
}

void CTextureManager::ReLoad()
{
}

void CTextureManager::Reset()
{
	for(int i=0;i<iTexturePacket.getSize();i++)
	{
		stTexturePacket* packet=iTexturePacket[i];
		GX_DELETE(packet);
	}
	iTexturePacket.clearAll();
    m_iTotalTextureMemory=0;
}