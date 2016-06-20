#pragma once

#include "object3d.h"
#include "gxTriInfo.h"
#include "gxMaterial.h"

class gxUV
{
public:
	gxUV()
	{
		material=NULL;
		textureCoordArray=NULL;
	}

	~gxUV()
	{
		material=NULL;
		GX_DELETE_ARY(textureCoordArray);
	}

	gxMaterial*     material;		//must not delete this pointer
	float*          textureCoordArray;
};


class gxMesh : public objectBase
{
public:
	gxMesh();
	virtual ~gxMesh();

	float* getVertexBuffer()	{	return vertexBuffer;	}
	float* getColorBuffer()		{	return colorBuffer;		}
	float* getNormalBuffer()	{	return normalBuffer;	}
	float* getTangentBuffer()	{	return tangentBuffer;	}

	virtual void update(float dt);
	virtual void render();
	void renderWithHWShader();

	float* allocateVertexBuffer(int nTris);
	float* allocateColorBuffer(int nTris);
	float* allocateNormalBuffer(int nTris);
	float* allocateTangentBuffer(int nTris);

	gxUV* allocateUVChannels(int nChannel, int nTris)
	{
		if(nChannel==0) return NULL;

		uvChannelCount=nChannel;
		uvChannel = new gxUV[nChannel];
		for(int x=0;x<nChannel;x++)
		{
			uvChannel[x].textureCoordArray = new float[nTris*3*2];
		}

		return uvChannel;
	}

	gxTriInfo* allocateTriInfoArray(int nCount)		{	triangleInfoArrayCount=nCount; triangleInfoArray=new gxTriInfo[nCount]; return triangleInfoArray; }
	int getNoOfTriInfo()							{	return triangleInfoArrayCount;				}
	gxTriInfo* getTriInfo(int index)				{	return &triangleInfoArray[index];	}

	int getVerticesCount();
	int getTriangleCount()		{    return noOfTrianglesForInternalUse;	}

	void loadTexture(const std::string& path, CTextureManager& textureManager);

	void setColor(vector3f clr)	{	color = clr;	}

protected:
    virtual void readScriptObject(gxFile& file);
	void renderNormal();

	bool applyStageTexture(unsigned int texID, gxUV* uv);
	void disableTextureOperations();

	int triangleInfoArrayCount;
	gxTriInfo* triangleInfoArray;

	float* vertexBuffer;
	float* colorBuffer;
	float* normalBuffer;
	float* tangentBuffer;

	vector3f color;

	unsigned int textureID;
	int uvChannelCount;
	gxUV* uvChannel;
	int noOfTrianglesForInternalUse;
};