#ifndef _TEXTUREMANAGER_H_
#define _TEXTUREMANAGER_H_

#include "glincludes.h"

#include <string.h>
#include "../util/bxLinkedList.h"
#include "types.h"
#include "stTexturePacket.h"

class CTextureManager
{
public:
	CTextureManager();
	~CTextureManager();

	stTexturePacket*	LoadTexture(const char* aFileName, const char* aOpFileName=NULL);
	void	Reset();	
    void    ReLoad();   //reload all textures from physical memory
    
    unsigned int getTotalTextureMemory()    {   return m_iTotalTextureMemory;   }
    
private:
	bxLinkedList<stTexturePacket*>	iTexturePacket;
    unsigned int m_iTotalTextureMemory;
};

#endif