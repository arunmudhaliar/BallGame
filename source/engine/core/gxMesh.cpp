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
		else
			glColor3fv(&color.x);

		int nTexUsed=0;
		for(int m=0;m<uvChannelCount;m++)
		{
			gxUV* uv=&uvChannel[m];
			if(applyStageTexture(textureID, uv))
				nTexUsed++;
		}
		glDrawElements(GL_TRIANGLES, triInfo->getVerticesCount(), GL_UNSIGNED_INT, triInfo->getTriList());

		disableTextureOperations();
	}

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	glPopMatrix();
}

void gxMesh::loadTexture(const std::string& path, CTextureManager& textureManager)
{
	auto texture = textureManager.LoadTexture(path.c_str());
	if(texture)
	{
		textureID = texture->iTextureID;
	}
}

bool gxMesh::applyStageTexture(unsigned int texID, gxUV* uv)
{
	if(!uv) return false;

	glEnable(GL_TEXTURE_2D);
	//glActiveTexture(GL_TEXTURE0);
	//glClientActiveTexture(GL_TEXTURE0);
	glTexCoordPointer(2, GL_FLOAT, 0, uv->textureCoordArray);
	//glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	return true;
}

void gxMesh::disableTextureOperations()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
}

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