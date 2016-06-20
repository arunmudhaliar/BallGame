#pragma once

#include "../engine/core/Sprite2Dx.h"

class borderWall : public Sprite2Dx
{
public:
	borderWall();
	virtual ~borderWall();

	void init(CTextureManager& textureManager, float scale = 2.0f);
	void update(float dt);
	void drawWall();
	bool checkCollision(vector2f& newPos, float radius);

private:
	float m_fScale;
	vector2f closestPt[4];
	vector2f debugPt[4];
	vector2f debugLines[4];
	Sprite2Dx closestPtSprite[4];
};