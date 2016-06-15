#pragma once

#include "../SpriteManager/Sprite2Dx.h"

class targetTrail : public Sprite2Dx
{
public:
	targetTrail();
	virtual ~targetTrail();

	void init(CTextureManager& textureManager);
	void update(float dt);
	void drawTrail();

	void calculateTrail(vector2f startPos, vector2f endPos);
};