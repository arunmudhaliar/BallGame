#include "gxWhiteShader.h"
#include "../core/gxMesh.h"

gxWhiteShader::gxWhiteShader():
gxHWShader()
{

}

gxWhiteShader::~gxWhiteShader()
{
}

void gxWhiteShader::bind()
{
}

void gxWhiteShader::getUniformVars()
{
}

void gxWhiteShader::updateShaderVars(float dt)
{
}

void gxWhiteShader::inputShaderUniformVars(void* ptr)
{
}

void gxWhiteShader::inputShaderAttribVars(void* ptr)
{
}

void gxWhiteShader::renderThroughHWShader(objectBase* obj, const matrix4x4f* parentTM)
{
	if(obj->getBaseID()!=OBJ_ID_MESH) return;

	gxMesh* mesh = (gxMesh*)obj;


}
