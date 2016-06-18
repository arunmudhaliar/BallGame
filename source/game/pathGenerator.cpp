#include "pathGenerator.h"

pathGenerator::pathGenerator()
{
	m_iThresholdAngle = 10;
}

pathGenerator::~pathGenerator()
{

}

void pathGenerator::init(CTextureManager& textureManager)
{
	m_cPointSprite.loadTexture(&textureManager, getResourcePath("textures//circle2.png"));
}

void pathGenerator::doBeginPath()
{
	m_cPath.clear();
}

void pathGenerator::doPath(float x, float y)
{
	if (m_cPath.size() < 2)
	{
		if (m_cPath.size() == 0)
		{
			m_cPath.push_back(vector2f(x, y));
			return;
		}

		auto diff = m_cPath[0] - vector2f(x, y);
		if (diff.length() > 5)
		{
			m_cPath.push_back(vector2f(x, y));
		}
		return;
	}

	if (isAngleUnderThreshold(x, y))
	{
		m_cPath.push_back(vector2f(x, y));
	}
}

void pathGenerator::doEndPath()
{
	m_cPath.clear();
}

void pathGenerator::drawPath()
{
	matrix4x4f temp;
	for (auto pt : m_cPath)
	{
		m_cPointSprite.draw(temp, &pt);
	}
}

bool pathGenerator::isAngleUnderThreshold(float x, float y)
{
	auto path_sz = m_cPath.size();
	if (path_sz < 2)
		return true;

	auto startPosA = m_cPath[path_sz - 2];
	auto endPosA = m_cPath[path_sz - 1];
	auto endPosB = vector2f(x, y);

	auto diffA = endPosA - startPosA;
	auto diffB = endPosA - endPosB;
	auto lengthB = diffB.length();

	if (lengthB < 0.00001f) return false;

	diffB.normalize();
	diffA.normalize();

	auto dot = diffA.dot(diffB);
	auto det = diffA.x*diffB.y - diffA.y*diffB.x;

	auto rotation = 180 - RAD2DEG(atan2(det, dot));

	return (rotation >= m_iThresholdAngle);
}

vector2f pathGenerator::getTop()
{
	if (isAnyPath())
		return m_cPath[0];

	return vector2f();
}