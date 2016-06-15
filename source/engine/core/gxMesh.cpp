#include "gxMesh.h"
#include "Timer.h"

gxMesh::gxMesh():
	objectBase(3000)
{
	triangleInfoArrayCount=0;
	triangleInfoArray=NULL;

	vertexBuffer=NULL;
	colorBuffer=NULL;
	normalBuffer=NULL;
	tangentBuffer=NULL;
	uvChannelCount=0;
	uvChannel=NULL;
	noOfTrianglesForInternalUse=0;
}

gxMesh::~gxMesh()
{
	triangleInfoArrayCount=0;
	GX_DELETE_ARY(triangleInfoArray);
	GX_DELETE_ARY(vertexBuffer);
	GX_DELETE_ARY(colorBuffer);
	GX_DELETE_ARY(normalBuffer);
	GX_DELETE_ARY(tangentBuffer);

	GX_DELETE_ARY(uvChannel);
}

void gxMesh::update(float dt)
{
    objectBase::update(dt);
}

void gxMesh::render()
{
	renderNormal();
}


void gxMesh::renderNormal()
{
	glPushMatrix();
	const float* mat = getMatrix();
	glMultMatrixf(mat);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertexBuffer);

	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, normalBuffer);

	for(int x=0;x<triangleInfoArrayCount;x++)
	{
		gxTriInfo* triInfo=&triangleInfoArray[x];
		if(!triInfo->getTriList()) continue;

		if(triInfo->getMaterial())
			glColor4fv(&triInfo->getMaterial()->getDiffuseClr().x);

		//int nTexUsed=0;
		//for(int m=0;m<uvChannelCount;m++)
		//{
		//	gxUV* uv=&uvChannel[m];
		//	if(triInfo->getMaterial() && applyStageTexture(renderer, nTexUsed, triInfo, uv, GL_TEXTURE_ENV_MODE, GL_MODULATE, 2))
		//		nTexUsed++;
		//}
		glDrawElements(GL_TRIANGLES, triInfo->getVerticesCount(), GL_UNSIGNED_INT, triInfo->getTriList());

		//disableTextureOperations(nTexUsed, NULL, NULL);
	}

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	glPopMatrix();
}

#if 0
void gxMesh::renderWithLight(gxRenderer* renderer, object3d* light)
{

	if(light->getID()!=OBJECT3D_LIGHT)
		return;

	gxLight* light_ob=(gxLight*)light;

	matrix4x4f cMV = *renderer->getViewMatrix() * *getWorldMatrix();
	const float* u_modelview_m4x4=cMV.getMatrix();

	matrix4x4f noscaleMV(cMV);
	noscaleMV.noScale();
	matrix4x4f cMVInverse = noscaleMV.getInverse();
	cMVInverse.transpose();
	const float* u_modelview_inverse_m4x4=cMVInverse.getMatrix();

	matrix4x4f cMVP = *renderer->getViewProjectionMatrix() * *getWorldMatrix();
    const float* u_mvp_m4x4=cMVP.getMatrix();

	for(int x=0;x<triangleInfoArrayCount;x++)
	{
		gxTriInfo* triInfo=&triangleInfoArray[x];
		if(!triInfo->getTriList()) continue;

		gxMaterial* material=triInfo->getMaterial();
		if(!material) continue;
		gxHWShader* shader=material->getShaderPass(1);
		shader->enableProgram();

		light_ob->renderPass(renderer, shader);

		shader->sendUniformTMfv("GEAR_MODEL_MATRIX", getMatrix(), false, 4);
		matrix4x4f inv_model=*this;
		inv_model.inverse();
		shader->sendUniformTMfv("GEAR_MODEL_INVERSE", inv_model.getMatrix(), false, 4);

		//shader->sendUniformTMfv("GEAR_MODELVIEW", u_modelview_m4x4, false, 4);
		shader->sendUniformTMfv("GEAR_MVP", u_mvp_m4x4, false, 4);
		//shader->sendUniformTMfv("GEAR_NORMAL_MATRIX", u_modelview_inverse_m4x4, false, 4);

		glVertexAttribPointer(shader->getAttribLoc("vIN_Position"), 3, GL_FLOAT, GL_FALSE, 0, getVertexBuffer());
		glEnableVertexAttribArray(shader->getAttribLoc("vIN_Position"));

		glVertexAttribPointer(shader->getAttribLoc("vIN_Normal"), 3, GL_FLOAT, GL_FALSE, 0, getNormalBuffer());
		glEnableVertexAttribArray(shader->getAttribLoc("vIN_Normal"));

		glVertexAttribPointer(shader->getAttribLoc("Tangent"), 3, GL_FLOAT, GL_FALSE, 0, getTangentBuffer());
		glEnableVertexAttribArray(shader->getAttribLoc("Tangent"));

		if(triInfo->getMaterial())
		{
			shader->sendUniform4fv("material.diffuse", &material->getDiffuseClr().x);
			shader->sendUniform4fv("material.ambient", &material->getAmbientClr().x);
			shader->sendUniform4fv("material.specular", &material->getSpecularClr().x);
			shader->sendUniform1f("material.shininess", 10.0f/*material->getShininess()*/);
		}
		else
		{
			shader->sendUniform4f("material.diffuse", 0.5f, 0.5f, 0.5f, 1.0f);
			shader->sendUniform4f("material.ambient", 0.2f, 0.2f, 0.2f, 1.0f);
			shader->sendUniform4f("material.specular", 0.2f, 0.2f, 0.2f, 1.0f);
			shader->sendUniform1f("material.shininess", 10.0f/*material->getShininess()*/);
		}


		glDrawElements(GL_TRIANGLES, triInfo->getVerticesCount(), GL_UNSIGNED_INT, triInfo->getTriList());
		renderer->noOfTrianglesRendered+=triInfo->getTriangleCount();
		renderer->noOfDrawCalls++;

		glDisableVertexAttribArray(shader->getAttribLoc("vIN_Normal"));
		glDisableVertexAttribArray(shader->getAttribLoc("vIN_Position"));

		shader->disableProgram();
	}

}
#endif

#if 0
bool gxMesh::applyStageTexture(gxRenderer* renderer, int stage, gxTriInfo* triInfo, gxUV* uv, gxSubMap* submap, int aTexEnv1, int aTexEnv2, unsigned int texCoordSz, gxHWShader* shader, const char* texCoordAttribName)
{
	if(!shader) return false;

	bool bUse1x1Texture=true;
	if(triInfo->getMaterial())
	{
		if(submap && submap->getTexture())
			bUse1x1Texture=false;
	}

	gxHWShader* hwShader=(gxHWShader*)shader;
    CHECK_GL_ERROR(glActiveTexture(GL_TEXTURE0+stage));
	if(bUse1x1Texture)
		CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, renderer->getGEARTexture1x1()->textureID));	
	else
	{
		CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, submap->getTexture()->getTextureID()));	
#if defined(TEXENV_ISSUE) && defined(GEAR_WINDOWS)
		glTexEnvf(GL_TEXTURE_ENV, aTexEnv1, (float)aTexEnv2);
#endif
		if(submap->getTexture()->getTextureType()==gxTexture::TEX_ALPHA)
		{
			//need to fix this bug
			CHECK_GL_ERROR(glEnable(GL_BLEND));
			CHECK_GL_ERROR(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
            getAttachedObject()->setBaseFlag(object3d::eObject3dBaseFlag_Alpha);	//check this
			//
		}
	}

	if(uv)
	{
		if(is_VBO)
		{
			CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, uv->vboID));
			CHECK_GL_ERROR(glVertexAttribPointer(hwShader->getAttribLoc(texCoordAttribName), 2, GL_FLOAT, GL_FALSE, 0, 0));
		}
		else
		{
			CHECK_GL_ERROR(glVertexAttribPointer(hwShader->getAttribLoc(texCoordAttribName), 2, GL_FLOAT, GL_FALSE, 0, uv->textureCoordArray));
		}
		CHECK_GL_ERROR(glEnableVertexAttribArray(hwShader->getAttribLoc(texCoordAttribName)));
	}
	//glEnable(GL_TEXTURE_2D);


	return true;
}

void gxMesh::disableTextureOperations(int stage, gxHWShader* shader, const char* texCoordAttribName)
{
#if defined (USE_ProgrammablePipeLine)
	if(shader)
	{
		CHECK_GL_ERROR(glActiveTexture(GL_TEXTURE0+stage));
		CHECK_GL_ERROR(glDisableVertexAttribArray(shader->getAttribLoc(texCoordAttribName)));
		//glDisable(GL_TEXTURE_2D);
		CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, 0));
		//glDisable(GL_TEXTURE_2D);
	}
#else
	//Disabling all texture operations
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	for(int i=0;i<nMultiTextureUsed;i++)
	{
		glActiveTexture(GL_TEXTURE0+i);
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glDisable(GL_TEXTURE_2D);	
	}
#endif
	//need to fix this bug
	CHECK_GL_ERROR(glDisable(GL_BLEND));
	//
}
#endif

float* gxMesh::allocateVertexBuffer(int nTris)
{
	noOfTrianglesForInternalUse=nTris;
	GX_DELETE_ARY(vertexBuffer);
	vertexBuffer = new float[nTris*3*3];
	return vertexBuffer;
}

float* gxMesh::allocateColorBuffer(int nTris)
{
	GX_DELETE_ARY(colorBuffer);
	colorBuffer = new float[nTris*3*3];
	return colorBuffer;
}

float* gxMesh::allocateNormalBuffer(int nTris)
{
	GX_DELETE_ARY(normalBuffer);
	normalBuffer = new float[nTris*3*3];
	return normalBuffer;
}

float* gxMesh::allocateTangentBuffer(int nTris)
{
	GX_DELETE_ARY(tangentBuffer);
	tangentBuffer = new float[nTris*3*4];
	return tangentBuffer;
}

int gxMesh::getVerticesCount()
{
	int nVerts=0;
	for(int x=0;x<triangleInfoArrayCount;x++)
	{
		nVerts+=triangleInfoArray[x].getVerticesCount();
	}

	return nVerts;
}

void gxMesh::readScriptObject(gxFile& file)
{
	file.Read(triangleInfoArrayCount);
	if(triangleInfoArrayCount)
		triangleInfoArray = new gxTriInfo[triangleInfoArrayCount];

	for(int x=0;x<triangleInfoArrayCount;x++)
	{
		triangleInfoArray[x].read(file);
	}

	file.Read(noOfTrianglesForInternalUse);

	bool bVertexBuffer=false;
	file.Read(bVertexBuffer);
	if(bVertexBuffer)
	{
		float* buffer=allocateVertexBuffer(noOfTrianglesForInternalUse);
		file.ReadBuffer((unsigned char*)buffer, sizeof(float)*noOfTrianglesForInternalUse*3*3);
	}

	bool bColorBuffer=false;
	file.Read(bColorBuffer);
	if(bColorBuffer)
	{
		float* buffer=allocateColorBuffer(noOfTrianglesForInternalUse);
		file.ReadBuffer((unsigned char*)buffer, sizeof(float)*noOfTrianglesForInternalUse*3*3);
	}

	bool bNormalBuffer=false;
	file.Read(bNormalBuffer);
	if(bNormalBuffer)
	{
		float* buffer=allocateNormalBuffer(noOfTrianglesForInternalUse);
		file.ReadBuffer((unsigned char*)buffer, sizeof(float)*noOfTrianglesForInternalUse*3*3);
	}

	bool bTangentBuffer=false;
	file.Read(bTangentBuffer);
	if(bTangentBuffer)
	{
		float* buffer=allocateTangentBuffer(noOfTrianglesForInternalUse);
		file.ReadBuffer((unsigned char*)buffer, sizeof(float)*noOfTrianglesForInternalUse*3*4);
	}


	file.Read(uvChannelCount);
	if(uvChannelCount)
		uvChannel = new gxUV[uvChannelCount];
	for(int x=0;x<uvChannelCount;x++)
	{
		int materialCRC=0;
		file.Read(materialCRC);
		uvChannel[x].textureCoordArray =new float[noOfTrianglesForInternalUse*3*2];
		file.ReadBuffer((unsigned char*)uvChannel[x].textureCoordArray, sizeof(float)*noOfTrianglesForInternalUse*3*2);
	}
}