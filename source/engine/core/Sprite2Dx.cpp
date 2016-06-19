#include "Sprite2Dx.h"

Sprite2Dx::Sprite2Dx():
Sprite()
{
}

Sprite2Dx::~Sprite2Dx()
{
	
}

void Sprite2Dx::draw(const matrix4x4f& parentTM, vector2f* pos)
{	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, m_cszVertLst);
    
	if (m_cTexture.getTextureType() != gxTexture::TEX_UNDEFINED)
	{
		glEnable(GL_TEXTURE_2D);
		//glActiveTexture(GL_TEXTURE0);
		//glClientActiveTexture(GL_TEXTURE0);
		glTexCoordPointer(2, GL_FLOAT, 0, m_cszTexCoord);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_cTexture.getTextureID());
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, (isRenderFlag(RENDER_FORCE_NO_MODULATION)&&m_fAlpha==1.0f)?GL_REPLACE:GL_MODULATE);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		
		if(m_cTexture.getTextureType()==gxTexture::TEX_ALPHA && !isRenderFlag(RENDER_FORCE_NO_ALPHA))
		{
 			glEnable(GL_BLEND);
			if(m_eBlendFlag==BLEND_MULTIPLY)
			{
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}
			else if(m_eBlendFlag==BLEND_ADDITIVE)
			{
				//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				glBlendFunc(GL_ONE, GL_ONE);
			}
		}
	}	
    
    if(!isRenderFlag(RENDER_FORCE_NO_ALPHA | RENDER_FORCE_NO_BLEND))
    {
        glColor4f(m_fRGB[0], m_fRGB[1], m_fRGB[2], m_fAlpha);
    }
    
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadMatrixf(m_cTextureMatrix.getMatrix());

	glMatrixMode(GL_MODELVIEW);
    if(!isRenderFlag(RENDER_FORCE_NO_TRANSFROM))
    {
        matrix4x4f cRenderMatrix;
        if(pos)
            cRenderMatrix.setPosition(pos->x, pos->y, 0.0f);
        else
            cRenderMatrix = (/*parentTM **/ (matrix4x4f)(*this));

		glPushMatrix();
        glMultMatrixf(cRenderMatrix.getMatrix());
    }
    
    onSpritePreRender();

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    onSpritePostRender();
    
	if (!isRenderFlag(RENDER_FORCE_NO_TRANSFROM))
	{
		glPopMatrix();
	}

	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	if (m_cTexture.getTextureType() != gxTexture::TEX_UNDEFINED)
	{	
		glDisable(GL_BLEND);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		//glActiveTexture(GL_TEXTURE0);
		glDisable(GL_TEXTURE_2D);
	}
	
	glDisableClientState(GL_VERTEX_ARRAY);
}

void Sprite2Dx::drawVerextBufferOnly(vector2f* pos)
{
#if defined (USE_ProgrammablePipeLine)
    
#else
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
#endif
}

