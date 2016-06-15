#pragma once

#include "types.h"

#include "gxMaterial.h"

class gxTriInfo
{
public:
	gxTriInfo()
	{
		vertexCount=0;
		triangleCount=0;
		triangleList=NULL;
		material=NULL;
	}

	~gxTriInfo()
	{
		GX_DELETE_ARY(triangleList);
	}

	int getVerticesCount()	{	return vertexCount;		}
	int getTriangleCount()	{   return triangleCount;	}

	int* getTriList()	{	return triangleList;	}

	int* allocateMemory(int nVertices)
	{
		vertexCount = nVertices;
		triangleList = new int[vertexCount*3];
		return triangleList;
	}

	gxMaterial* getMaterial()				{	return material;            }
	void setMaterial(gxMaterial* material)
    {
        if(this->material==material)
            return;
        this->material = material;
    }

	void read(gxFile& file)
	{
		file.Read(vertexCount);
		triangleCount=vertexCount/3;
		if(vertexCount)
		{
			int* buffer = allocateMemory(vertexCount);
			file.ReadBuffer((unsigned char*)buffer, sizeof(int)*vertexCount*3);
		}
		int materialCRC;
		file.Read(materialCRC);
	}

private:
	gxMaterial* material;	//must not delete this pointer

	int vertexCount;
	int triangleCount;
	int* triangleList;
};