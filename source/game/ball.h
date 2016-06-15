#pragma once

#include "entityManager.h"
#include "../engine/core/gxMesh.h"

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
	void addForce(vector3f force);

private:
	void readHeader(gxFile& file);

	vector3f m_cForce;
	vector3f m_cVelocity;
};