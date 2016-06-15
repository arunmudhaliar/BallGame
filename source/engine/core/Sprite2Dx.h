#ifndef SPRITE2DX_H
#define SPRITE2DX_H

//#include "CommonData.h"
#include "Sprite.h"

class Sprite2Dx : public Sprite
{
public:	
	Sprite2Dx();
	~Sprite2Dx();
	
	virtual void draw(const matrix4x4f& parentTM, vector2f* pos=NULL);
    
    virtual void onSpritePreRender(){};
    virtual void onSpritePostRender(){};

    void drawVerextBufferOnly(vector2f* pos=NULL);
};

#endif

