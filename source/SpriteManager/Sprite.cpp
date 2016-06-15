#include "Sprite.h"

Sprite::Sprite():
transformf()
{
	m_cClipWidth=0;
	m_cClipHeight=0;
	m_cClipX=m_cClipY=0;
    
    clearRenderFlag();
    setRenderFlag(RENDER_NORMAL);
    
	setOffset(-1.0f, 1.0f);
    setAlpha(1.0f);
    setRGB(1.0f, 1.0f, 1.0f);
    setAlphaThreshold(1.0f);
	setBlendMode(BLEND_MULTIPLY);
}

Sprite::~Sprite()
{
	
}

void Sprite::loadTexture(CTextureManager* textureManager, const char *pszFileName)
{
	stTexturePacket *tp = textureManager->LoadTexture(pszFileName); 
	m_cTexture.setTexture(tp);
	
	if(tp->bAlphaTex)
		m_cTexture.setTextureType(gxTexture::TEX_ALPHA);
	else
		m_cTexture.setTextureType(gxTexture::TEX_NORMAL);
	
	m_cOrigSize.x = (float)tp->m_cWidth;
	m_cOrigSize.y = (float)tp->m_cHeight;
	
	setClip(0, 0, m_cOrigSize.x, m_cOrigSize.y);
}

void Sprite::setTile(float tileX, float tileY)
{
	m_cTextureMatrix.setScale(tileX, tileY, 1.0f);
}

void Sprite::recalculateTile()
{
	vector3f xaxis(m[0], m[1], m[2]);
	vector3f yaxis(m[4], m[5], m[6]);
	vector3f zaxis(m[8], m[9], m[10]);

	auto sx = xaxis.length();
	auto sy = yaxis.length();
	auto sz = zaxis.length();
	m_cTextureMatrix.setScale(sx, sy, sz);
}

void Sprite::setClip(float clipX, float clipY, float width, float height)
{
    m_cClipX=clipX;
    m_cClipY=clipY;
    
	m_cClipWidth=width;
	m_cClipHeight=height;
	
	float offX=(m_cOffset.x+1.0f)*0.5f;
	float offY=(-m_cOffset.y+1.0f)*0.5f;	
	
	const float vertLst[8] =
	{
		(width * (1.0f-offX)),		(height * (-offY)),
		(width * (-offX)),			(height * (-offY)),
		(width * (1.0f-offX)),		(height * (1.0f-offY)),
		(width * (-offX)),			(height * (1.0f-offY)),
	};
	
	memcpy(m_cszVertLst, vertLst, sizeof(vertLst));
	
	if(m_cOrigSize.x<=0.0f)	m_cOrigSize.x = 1.0f;
	if(m_cOrigSize.y<=0.0f)	m_cOrigSize.y = 1.0f;
	
	float min_x=(float)clipX/m_cOrigSize.x;
	float min_y=(float)clipY/m_cOrigSize.y;
	float max_x=(float)(clipX+width)/m_cOrigSize.x;
	float max_y=(float)(clipY+height)/m_cOrigSize.y;
	
	vector2f aGLmin(min_x, min_y);
	vector2f aGLmax(max_x, max_y);

	const float tex[8] =
	{
		(aGLmax.x)	, (aGLmin.y),
		(aGLmin.x)	, (aGLmin.y),
		(aGLmax.x)	, (aGLmax.y),
		(aGLmin.x)	, (aGLmax.y),
	};
	
	memcpy(m_cszTexCoord, tex, sizeof(tex));
}

void Sprite::setVFlip()
{
    GX_SWAP_FLOAT(m_cszTexCoord[0], m_cszTexCoord[4]);
    GX_SWAP_FLOAT(m_cszTexCoord[1], m_cszTexCoord[5]);
    GX_SWAP_FLOAT(m_cszTexCoord[2], m_cszTexCoord[6]);
    GX_SWAP_FLOAT(m_cszTexCoord[3], m_cszTexCoord[7]);
}

void Sprite::setHFlip()
{
    GX_SWAP_FLOAT(m_cszTexCoord[0], m_cszTexCoord[2]);
    GX_SWAP_FLOAT(m_cszTexCoord[1], m_cszTexCoord[3]);
    GX_SWAP_FLOAT(m_cszTexCoord[4], m_cszTexCoord[6]);
    GX_SWAP_FLOAT(m_cszTexCoord[5], m_cszTexCoord[7]);
}

void Sprite::setOffset(float x, float y)
{
	m_cOffset.x=x;
	m_cOffset.y=y;
}

void Sprite::debugDraw(vector2f* pos)
{
    glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, m_cszVertLst);
    
    glPushMatrix();
	if(pos)
		glTranslatef(pos->x, pos->y, 0.0f);
	else
		glMultMatrixf(m);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
	glPopMatrix();
    
   	glDisableClientState(GL_VERTEX_ARRAY);
}
