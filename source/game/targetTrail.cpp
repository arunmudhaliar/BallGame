#include "targetTrail.h"

targetTrail::targetTrail():
	Sprite2Dx()
{
	m_fScale = 1.0f;
	m_bVisible = true;
}

targetTrail::~targetTrail()
{
}

void targetTrail::init(CTextureManager& textureManager)
{
	setOffset(1, 0);
	loadTexture(&textureManager, getResourcePath("textures//arrow-test.png"));
}

void targetTrail::calculateTrail(vector2f startPos, vector2f endPos)
{
	auto diff = endPos - startPos;
	auto length = diff.length();

	if (length < 0.00001f) return;

	vector2f median(1, 0);
	diff.normalize();

	auto dot = diff.dot(median);
	auto det = diff.x*median.y - diff.y*median.x;

	auto scale = length / getClipWidth();
	m_fScale = scale;
	auto rotation = 180 - RAD2DEG(atan2(det, dot));

	matrix4x4f rotationTM;
	rotationTM.setRotationMatrix(rotation, false, false, true);
	matrix4x4f scaleTM;
	scaleTM.setScale(scale, 1.0f, 0.0f);
	matrix4x4f translateTM;
	translateTM.setPosition(vector3f(startPos.x, startPos.y, 0.0f));
	auto finalTM = translateTM * rotationTM * scaleTM;
	copy(finalTM);
	recalculateTile();
}

void targetTrail::update(float dt)
{
	auto temp = m_cTextureMatrix.getMatrix();
	auto pos = m_cTextureMatrix.getPosition();
	float delta = 0.3f/m_fScale * dt;
	pos.x += (delta* temp[0]);
	pos.y += (delta* temp[1]);
	pos.z += (delta* temp[2]);

	m_cTextureMatrix.setPosition(pos);
}

void targetTrail::drawTrail()
{
	if(m_bVisible)
	{
		draw(*objectBase::getRenderer()->getViewMatrix());
	}
}