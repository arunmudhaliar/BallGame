#ifndef GXDIFFUSEMAPLIGHTMAP_H
#define GXDIFFUSEMAPLIGHTMAP_H

#include "gxHWShader.h"

class gxDiffuseMapLightMap : public gxHWShader
{
public:
	
	gxDiffuseMapLightMap();
	~gxDiffuseMapLightMap();
	
	virtual void bind();
	virtual void getUniformVars();
	virtual void updateShaderVars(float dt);
	virtual void inputShaderUniformVars(void* ptr);
	virtual void inputShaderAttribVars(void* ptr);

	GLint getUserDefinedUniform(int index)		{	return 0;	}
	
	virtual void renderThroughHWShader(objectBase* obj, const matrix4x4f* parentTM);
};
#endif