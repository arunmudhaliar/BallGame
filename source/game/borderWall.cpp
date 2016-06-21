#include "borderWall.h"
#include "../engine/core/object3d.h"
#include "../engine/util/util.h"
#include "../engine/sceneManager/Scene.h"

borderWall::borderWall():
Sprite2Dx()
{
}

borderWall::~borderWall()
{
}

void borderWall::init(CTextureManager& textureManager, float scale)
{
	m_fScale = scale;
	setOffset(0, 0);
	loadTexture(&textureManager, getResourcePath("textures//Grass_2.png"));
	setScale(m_fScale, m_fScale, 1);
	recalculateTile();

#if DEBUG_COLLISION
	for(int x=0;x<4;x++)
	{
		closestPtSprite[x].setOffset(0, 0);
		closestPtSprite[x].loadTexture(&textureManager, getResourcePath("textures//star33.png"));
		closestPtSprite[x].setScale(0.25f, 0.25f, 1);
	}
#endif
}

void borderWall::update(float dt)
{
}

void borderWall::drawWall()
{
	draw(*objectBase::getRenderer()->getViewMatrix());

#if DEBUG_COLLISION
	for(int x=0;x<4;x++)
	{
		closestPtSprite[x].set2DPosition(closestPt[x].x, closestPt[x].y);
		closestPtSprite[x].draw(*objectBase::getRenderer()->getViewMatrix());
	}

	glBegin(GL_LINE_LOOP);
	glVertex2f(debugLines[0].x, debugLines[0].y);
	glVertex2f(debugLines[1].x, debugLines[1].y);
	glVertex2f(debugLines[2].x, debugLines[2].y);
	glVertex2f(debugLines[3].x, debugLines[3].y);
	glEnd();
#endif
}

bool borderWall::checkCollision(vector2f& newPos, float radius)
{
	auto viewportSz = Scene::getCommonData()->getRendererPtr()->getViewPortSz();

	bool bCollided = false;
	int collision_check_cntr = 5;

	float radiusSq = radius * radius;
	//circle-rectangle collision
	float multiplier = m_fScale*0.5f;

	vector2f left(viewportSz.x*0.5f-getClipWidth()*multiplier, viewportSz.y*0.5f+getClipHeight()*multiplier);
	vector2f right(viewportSz.x*0.5f+getClipWidth()*multiplier, viewportSz.y*0.5f-getClipHeight()*multiplier);
	vector2f top(viewportSz.x*0.5f-getClipWidth()*multiplier, viewportSz.y*0.5f-getClipHeight()*multiplier);
	vector2f bottom(viewportSz.x*0.5f+getClipWidth()*multiplier, viewportSz.y*0.5f+getClipHeight()*multiplier);

#if DEBUG_COLLISION
	debugLines[0] = left;
	debugLines[1] = right;
	debugLines[2] = top;
	debugLines[3] = bottom;
#endif

	while (collision_check_cntr--)
	{
		//oldPos = newPos;
		vector2f avgPos;
		int cnt = 0;

		//check for penitration
		bool bPenitration = true;
		if (newPos.x>left.x || newPos.y>top.y || newPos.x<right.x || newPos.y<bottom.y)
			bPenitration = false;

		if(bPenitration)
		{
		    DEBUG_PRINT("=======================PENITRATION OCCURED=================");
		}

		closestPt[0] = (gxUtil::closestPointOnLine(newPos, left, top));
		closestPt[1] = (gxUtil::closestPointOnLine(newPos, top, right));
		closestPt[2] = (gxUtil::closestPointOnLine(newPos, right, bottom));
		closestPt[3] = (gxUtil::closestPointOnLine(newPos, bottom, left));

#if DEBUG_COLLISION
		debugPt[0] = closestPt[0];
		debugPt[1] = closestPt[1];
		debugPt[2] = closestPt[2];
		debugPt[3] = closestPt[3];
#endif

		//which one is the closest
		float closest_length = (float)GX_MAX_INT;
		int closest_index = -1;

		for (int l = 0; l < 4; l++)
		{
			vector2f diff(newPos - closestPt[l]);
			float length = diff.lengthSquared();
			if (length < closest_length)
			{
				closest_length = length;
				closest_index = l;

				if (bPenitration)
				{
					diff = -diff;
				}

				if (closest_length <= radiusSq)
				{
					//collision occured
					diff.normalize();
					float val = radius + 0.1f;
					vector2f calc_Pos(closestPt[closest_index] + (diff*val));
					avgPos += calc_Pos;
					cnt++;
					//DEBUG_PRINT("(%d), closestPt(%f, %f), calc_pos(%f, %f)", l, closestPtf[closest_index].x, closestPtf[closest_index].y, calc_Pos.x, calc_Pos.y);
				}
			}
		}//for

		if (cnt)
		{
			bCollided = true;
			avgPos.x = avgPos.x / cnt;
			avgPos.y = avgPos.y / cnt;
			newPos = avgPos;	//this will cause the actor to come to a halt then move away, so i commented this line
			DEBUG_PRINT("collision occured %d, avgPos(%f, %f)", cnt, avgPos.x, avgPos.y);
		}
		else
		{
			break;	//break the loop
		}
	}

	return bCollided;
}