#ifndef GXWHITESHADER_H
#define GXWHITESHADER_H

#include "gxHWShader.h"

class gxWhiteShader : public gxHWShader
{
public:
	
	gxWhiteShader();
	~gxWhiteShader();
	
	virtual void bind();
	virtual void getUniformVars();
	virtual void updateShaderVars(float dt);
	virtual void inputShaderUniformVars(void* ptr);
	virtual void inputShaderAttribVars(void* ptr);

	GLint getUserDefinedUniform(int index)		{	return 0;	}
	
	virtual void renderThroughHWShader(objectBase* obj, const matrix4x4f* parentTM);
};
#endif