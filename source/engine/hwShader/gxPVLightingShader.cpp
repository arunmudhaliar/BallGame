#include "gxPVLightingShader.h"
#include "../core/gxMesh.h"

gxPVLightingShader::gxPVLightingShader():
gxHWShader()
{

}

gxPVLightingShader::~gxPVLightingShader()
{
}

void gxPVLightingShader::bind()
{
	// Bind attribute locations
	// this needs to be done prior to linking
	glBindAttribLocation(getProgram(), ATTRIB_VERTEX_COORD,		"a_vertex_coord_v4");
	glBindAttribLocation(getProgram(), ATTRIB_UV_COORD,			"a_uv_coord_v2");
	glBindAttribLocation(getProgram(), ATTRIB_NORMAL_COORD,		"a_normal_coord_v3");
	glBindAttribLocation(getProgram(), ATTRIB_DIFFUSE_COLOR,	"a_diffuse_v3");
	glBindAttribLocation(getProgram(), ATTRIB_AMBIENT_COLOR,	"a_ambient_v3");
	glBindAttribLocation(getProgram(), ATTRIB_SPECULAR_COLOR,	"a_specular_v3");
	glBindAttribLocation(getProgram(), ATTRIB_SHININESS_FACTOR, "a_shininess_f1");	
}

void gxPVLightingShader::getUniformVars()
{
	// Get uniform locations
	m_cUniforms[UNIFORM_MODELVIEW_TM]			= glGetUniformLocation(getProgram(), "u_modelview_m4x4");
	m_cUniforms[UNIFORM_MODELVIEWPROJECTION_TM]	= glGetUniformLocation(getProgram(), "u_mvp_m4x4");
	m_cUniforms[UNIFORM_LIGHT_POSITION]			= glGetUniformLocation(getProgram(), "u_lightposition_v3");
	m_cUniforms[UNIFORM_EYE_DIRECTION]			= glGetUniformLocation(getProgram(), "u_eyedirection_v3");
	
	m_cUniforms[UNIFORM_FLAGS]					= glGetUniformLocation(getProgram(), "u_flags_m4x4");
}

void gxPVLightingShader::updateShaderVars(float dt)
{	
//	//update attrib vars
//	glVertexAttrib3fv(ATTRIB_DIFFUSE_COLOR, m_pAttribInputPtr->a_diffuse_v3);
//	glVertexAttrib3fv(ATTRIB_AMBIENT_COLOR, m_pAttribInputPtr->a_ambient_v3);
//	glVertexAttrib3fv(ATTRIB_SPECULAR_COLOR, m_pAttribInputPtr->a_specular_v3);
//	glVertexAttrib1f(ATTRIB_SHININESS_FACTOR, *m_pAttribInputPtr->a_shininess_f1);
//
//	glUniformMatrix4fv(m_cUniforms[UNIFORM_MODELVIEW_TM], 1, false, m_pUniformInputPtr->u_modelview_m4x4);
//	glUniformMatrix4fv(m_cUniforms[UNIFORM_MODELVIEWPROJECTION_TM], 1, false, m_pUniformInputPtr->u_mvp_m4x4);
//	glUniform3fv(m_cUniforms[UNIFORM_EYE_DIRECTION], 1, m_pUniformInputPtr->u_eyedirection_v3);
//	glUniformMatrix4fv(m_cUniforms[UNIFORM_FLAGS], 1, false, m_pUniformInputPtr->u_flags_m4x4);
}

void gxPVLightingShader::inputShaderUniformVars(void* ptr)
{
//	m_pUniformInputPtr=(stCommonShaderUniformInput*)ptr;
}

void gxPVLightingShader::inputShaderAttribVars(void* ptr)
{
//	m_pAttribInputPtr=(stCommonShaderAttribInput*)ptr;
}

void gxPVLightingShader::renderThroughHWShader(objectBase* obj, const matrix4x4f& parentTM)
{
	if(obj->getBaseID()!=OBJ_ID_MESH) return;

	gxMesh* mesh = (gxMesh*)obj;


}
