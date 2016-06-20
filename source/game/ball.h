#pragma once

#include "../engine/core/gxMesh.h"

#define BALL_RADIUS	20.0f

class ball : public gxMesh
{
public:
	ball();
	~ball();

	void load(const std::string& path, const std::string& texturePath, CTextureManager& textureManager);
	void update(float dt);
	void render(const matrix4x4f& renderMatrix);

	//physics
	void clearForce();
	void addForce(vector2f force);
	float getRadius()		{	return BALL_RADIUS;	}
	vector2f getVelocity()	{	return m_cVelocity;	}
	//

	void turnRed()	{	m_fTurnRedColor = 1.0f;	m_bTurnRed = true;	}

private:
	void doTurnRed(float dt);
	void doPhysics(float dt);

	float m_fTurnRedColor;
	bool m_bTurnRed;

	vector2f m_cForce;
	vector2f m_cVelocity;
	
};