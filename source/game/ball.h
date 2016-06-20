#pragma once

#include "../engine/core/gxMesh.h"

#define BALL_RADIUS	10.0f;

class ball : public gxMesh
{
public:
	ball();
	~ball();

	void load(const std::string& path);
	void update(float dt);
	void render(const matrix4x4f& renderMatrix);

	void updatePhysics(float dt);
	void clearForce();
	void addForce(vector2f force);

	float getRadius()		{	return BALL_RADIUS;	}
	vector2f getVelocity()	{	return m_cVelocity;	}

private:
	void readHeader(gxFile& file);

	vector2f m_cForce;
	vector2f m_cVelocity;
};