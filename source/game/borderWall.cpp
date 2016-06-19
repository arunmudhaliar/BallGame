#include "borderWall.h"
#include "../engine/core/object3d.h"

borderWall::borderWall():
Sprite2Dx()
{
}

borderWall::~borderWall()
{
}

void borderWall::init(CTextureManager& textureManager)
{
	setOffset(0, 0);
	loadTexture(&textureManager, getResourcePath("textures//Grass_2.png"));
	setScale(2, 2, 1);
	recalculateTile();
}

void borderWall::update(float dt)
{
}

void borderWall::drawWall()
{
	draw(*objectBase::getRenderer()->getViewMatrix());
}