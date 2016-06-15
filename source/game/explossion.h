#pragma once

#include "entityManager.h"
#include "../engine/core/Sprite2Dx.h"
#include "../engine/core/TextureManager.h"

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