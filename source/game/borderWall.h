#pragma once

#include "../engine/core/Sprite2Dx.h"

//This class is responsible for constructing a border, which by default 2 times bigger than the loaded sprite.
//Also responsible for continous box-sphere collision.

#define DEBUG_COLLISION	0

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

#if DEBUG_COLLISION
	vector2f debugPt[4];
	vector2f debugLines[4];
	Sprite2Dx closestPtSprite[4];
#endif
};