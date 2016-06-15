#include "gxMaterial.h"

gxMaterial::gxMaterial()
{
	diffuseColor.set(0.5f, 0.5f, 0.5f);
	ambientColor.set(0.1f, 0.1f, 0.1f);
	specularColor.set(0.2f, 0.2f, 0.2f);
	alpha=1.0f;
	shininess=10.0f;
	twoSided=false;
}

gxMaterial::~gxMaterial()
{
}