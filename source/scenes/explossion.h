#ifndef EXPLOSSION_H
#define EXPLOSSION_H

#include "entityManager.h"
#include "../SpriteManager/Sprite2Dx.h"
#include "../engine/core/TextureManager.h"
//#include "../engine/core/vector2.h"

class explossion : public entityBase, public Sprite2Dx
{
public:
	explossion(CTextureManager* pTextureManager, const char* filename);
	~explossion();

	virtual void update(float dt);
	virtual void render(const matrix4x4f& renderMatrix);

private:
	float m_fCurrentFrame;
	int m_nFrames;
	vector2i m_cTextureSize;
	vector2i m_cFrameSize;
};

#endif