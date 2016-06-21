#include "pathGenerator.h"

pathGenerator::pathGenerator()
{
	m_iThresholdAngle = PATH_GENERATOR_THRESHOLD_ANGLE;
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
		if (diff.length() > m_iThresholdAngle)
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
}

void pathGenerator::drawPath()
{
	matrix4x4f temp;
	for (int x=0;x<(int)m_cPath.size();x++)
	{
		m_cPointSprite.draw(temp, &m_cPath[x]);
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

vector2f pathGenerator::getTop(int count)
{
	if (count > (int)m_cPath.size())
	{
		count = m_cPath.size();
	}

	vector2f outPut;
	for (int x = 0; x < count; x++)
	{
		outPut += m_cPath[x];
	}
	return outPut/count;
}

bool pathGenerator::isReachedNearTop(vector2f& pt, float collisionRadius, float& t, int count)
{
	t = 0.0f;
	if (!isAnyPath()) return false;

	auto diff = pt - getTop(count);
	auto lengthSq = diff.lengthSquared();
	auto collision_radiusSq = collisionRadius*collisionRadius;
	t = lengthSq / collision_radiusSq;
	return (lengthSq < collision_radiusSq);
}

void pathGenerator::popTop(int count)
{
	if (count > (int)m_cPath.size())
	{
		count = m_cPath.size();
	}

	for (int x = 0; x < count; x++)
	{
		m_cPath.erase(m_cPath.begin());
	}
}