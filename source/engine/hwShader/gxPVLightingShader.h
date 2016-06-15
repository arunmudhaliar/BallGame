#ifndef GXPVLIGHTINGSHADER_H
#define GXPVLIGHTINGSHADER_H

#include "gxHWShader.h"

class gxPVLightingShader : public gxHWShader
{
public:
	
	//uniform index
	enum
	{
		UNIFORM_MODELVIEW_TM,
		UNIFORM_MODELVIEWPROJECTION_TM,
		
		UNIFORM_LIGHT_POSITION,
		UNIFORM_EYE_DIRECTION,
		UNIFORM_FLAGS,
		UNIFORM_MAX
	};
	
	// attribute index
	enum
	{
		ATTRIB_VERTEX_COORD,
		ATTRIB_UV_COORD,
		ATTRIB_NORMAL_COORD,
		
		ATTRIB_DIFFUSE_COLOR,
		ATTRIB_AMBIENT_COLOR,
		ATTRIB_SPECULAR_COLOR,
		ATTRIB_SHININESS_FACTOR,
		ATTRIB_MAX
	};
	
	gxPVLightingShader();
	~gxPVLightingShader();
	
	virtual void bind();
	virtual void getUniformVars();
	virtual void updateShaderVars(float dt);
	virtual void inputShaderUniformVars(void* ptr);
	virtual void inputShaderAttribVars(void* ptr);

	GLint getUserDefinedUniform(int index)		{	return m_cUniforms[index];	}
	
	virtual void renderThroughHWShader(objectBase* obj, const matrix4x4f& parentTM);

	GLint m_cUniforms[UNIFORM_MAX];
	
//	//shader inputs
//	stCommonShaderUniformInput* m_pUniformInputPtr;	//must not delete this pointer
//	stCommonShaderAttribInput* m_pAttribInputPtr;	//must not delete this pointer
};
#endif