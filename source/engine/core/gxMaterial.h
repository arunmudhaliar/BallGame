#pragma once

#include "vector3.h"
#include "TextureManager.h"
#include <algorithm>
#include <string>

class gxMaterial
{
public:
    gxMaterial();
    ~gxMaterial();

	bool isTwoSided()                   {	return twoSided;	}

	const vector3f& getAmbientClr()		{	return ambientColor;	}
	const vector3f& getDiffuseClr()		{ return diffuseColor; }
	const vector3f& getSpecularClr()     { return specularColor; }

	void setAmbientClr(vector3f clr)     { ambientColor = clr; }
	void setDiffuseClr(vector3f clr)     { diffuseColor = clr; }
	void setSpecularClr(vector3f clr)	{ specularColor = clr; }

	float getShininess()				{	return shininess;	}
	void setShininess(float value)		{	shininess=value;	}

    void setMaterialName(const std::string& name)	{	materialName = name;	}
    const std::string& getMaterialName()            {	return materialName;    }

private:
	vector3f ambientColor;
	vector3f diffuseColor;
	vector3f specularColor;
	float alpha;
	float shininess;
	bool twoSided;
    std::string materialName;
};