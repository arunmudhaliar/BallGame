#pragma once

#include <vector>
#include "../engine/core/vector2.h"
#include "../engine/core/Sprite2Dx.h"

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

	vector2f getTop();

private:
	bool isAngleUnderThreshold(float x, float y);

	int m_iThresholdAngle;
	std::vector<vector2f> m_cPath;
	Sprite2Dx m_cPointSprite;
};