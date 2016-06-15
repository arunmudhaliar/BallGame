#ifndef GXDIFFUSEMAPUNLIT_H
#define GXDIFFUSEMAPUNLIT_H

#include "gxHWShader.h"

class gxDiffuseMapUnlit : public gxHWShader
{
public:
	
	gxDiffuseMapUnlit();
	~gxDiffuseMapUnlit();
	
	virtual void bind();
	virtual void getUniformVars();
	virtual void updateShaderVars(float dt);
	virtual void inputShaderUniformVars(void* ptr);
	virtual void inputShaderAttribVars(void* ptr);

	GLint getUserDefinedUniform(int index)		{	return 0;	}
	
	virtual void renderThroughHWShader(objectBase* obj, const matrix4x4f* parentTM);
    
    void renderAsNormalMesh(objectBase* obj, const matrix4x4f* parentTM);
    void renderAsVBOMesh(objectBase* obj, const matrix4x4f* parentTM);
};
#endif