#include "explossion.h"

explossion::explossion(CTextureManager* pTextureManager, const char* filename):
	entityBase(900),
	Sprite2Dx()
{
	setOffset(0, 0);
	loadTexture(pTextureManager, getResourcePath(filename));


	m_fCurrentFrame = 0.0f;
	m_nFrames = 30;
	m_cTextureSize.set(getOrigWidth(), getOrigHeight());
	m_cFrameSize.set(64, 64);
}

explossion::~explossion()
{
}

void explossion::update(float dt)
{
	float fps = 30.0f;
	float framesToSkip = fps*dt;
	m_fCurrentFrame+=framesToSkip;

	int nRows=m_cTextureSize.y/m_cFrameSize.y;
	int nCols=m_cTextureSize.x/m_cFrameSize.x;

	int currentFrame=(int)m_fCurrentFrame;
	if(currentFrame>=nRows*nCols || currentFrame>=m_nFrames)
	{
		m_fCurrentFrame=0.0f;
		setActive(false);
		entityBase::update(dt);
		return;
	}

	int current_row=currentFrame/nCols;
	int current_col=currentFrame%nCols;

	//vector2f startUV((current_col*m_cFrameSize.x)/m_cTextureSize.x, (current_row*m_cFrameSize.y)/m_cTextureSize.y);
	//vector2f endUV(startUV.x+m_cFrameSize.x/m_cTextureSize.x, startUV.y+m_cFrameSize.y/m_cTextureSize.y);

	setClip((float)current_col*m_cFrameSize.x, (float)current_row*m_cFrameSize.y, (float)m_cFrameSize.x, (float)m_cFrameSize.y);

	entityBase::update(dt);
}

void explossion::render(const matrix4x4f& renderMatrix)
{
	draw(renderMatrix);
}
