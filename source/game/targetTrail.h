#pragma once

#include "../engine/core/Sprite2Dx.h"
#include "../engine/core/object3d.h"

class targetTrail : public Sprite2Dx
{
public:
	targetTrail();
	virtual ~targetTrail();

	void init(CTextureManager& textureManager);
	void update(float dt);
	void drawTrail();

	void calculateTrail(vector2f startPos, vector2f endPos);

private:
	float m_fScale;
};