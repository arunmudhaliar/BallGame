#pragma once

#include "../engine/core/Sprite2Dx.h"
#include "../engine/core/object3d.h"

//This class is responsible for rendering the path with a small point sprite.

class targetTrail : public Sprite2Dx
{
public:
	targetTrail();
	virtual ~targetTrail();

	void init(CTextureManager& textureManager);
	void update(float dt);
	void drawTrail();

	void calculateTrail(vector2f startPos, vector2f endPos);

	void setVisible(bool bVisible)	{	m_bVisible = bVisible;	}
	bool isVisible()				{	return m_bVisible;		}

private:
	bool m_bVisible;
	float m_fScale;
};