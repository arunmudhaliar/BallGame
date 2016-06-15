//
//  gxFontShader.h
//  GEAR5
//
//  Created by arun on 03/04/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef GEAR5_gxFontShader_h
#define GEAR5_gxFontShader_h


#include "gxHWShader.h"

class gxFontShader : public gxHWShader
{
public:
	gxFontShader();
	~gxFontShader();
	
	virtual void bind();
	virtual void getUniformVars();
	virtual void updateShaderVars(float dt);
	virtual void inputShaderUniformVars(void* ptr);
	virtual void inputShaderAttribVars(void* ptr);
    
    virtual GLint getUserDefinedUniform(int index);
    
	virtual void renderThroughHWShader(objectBase* obj, const matrix4x4f* parentTM);
};

#endif
