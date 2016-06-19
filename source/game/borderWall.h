#pragma once

#include "../engine/core/Sprite2Dx.h"

class borderWall : public Sprite2Dx
{
public:
	borderWall();
	virtual ~borderWall();

	void init(CTextureManager& textureManager);
	void update(float dt);
	void drawWall();
};