#ifndef GXBLURSHADER_H
#define GXBLURSHADER_H

#include "gxHWShader.h"

class gxBlurShader : public gxHWShader
{
public:
	
	gxBlurShader();
	~gxBlurShader();
	
	virtual void bind();
	virtual void getUniformVars();
	virtual void updateShaderVars(float dt);
	virtual void inputShaderUniformVars(void* ptr);
	virtual void inputShaderAttribVars(void* ptr);

	GLint getUserDefinedUniform(int index)		{	return 0;	}
	
	virtual void renderThroughHWShader(objectBase* obj, const matrix4x4f* parentTM);
    
    float m_fAngle;
};
#endif