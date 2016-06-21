#pragma once

#include <vector>
#include "../engine/core/vector2.h"
#include "../engine/core/Sprite2Dx.h"

//This class is responsible for generating points based on the mouse drag event. 
//The generation of points are limited by the difference of angle between the new point and the previous point in the list.

#define PATH_GENERATOR_THRESHOLD_ANGLE 5
#define PATH_GENERATOR_NO_OF_POINTS_TO_APPROXIMATE	5

class pathGenerator
{
public:
	pathGenerator();
	~pathGenerator();

	void init(CTextureManager& textureManager);
	void doBeginPath();
	void doPath(float x, float y);
	void doEndPath();

	void drawPath();

	int getPathCount()	{ return m_cPath.size(); }
	bool isAnyPath()	{ return m_cPath.size() != 0;	}
	vector2f getTop(int count=1);
	bool isReachedNearTop(vector2f& pt, float collisionRadius, float& t, int count=1);
	void popTop(int count=1);

private:
	bool isAngleUnderThreshold(float x, float y);

	int m_iThresholdAngle;
	std::vector<vector2f> m_cPath;
	Sprite2Dx m_cPointSprite;
};