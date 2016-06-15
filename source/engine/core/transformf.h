
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "matrix4x4f.h"

class transformf : public matrix4x4f
{
public:
	
	transformf():matrix4x4f()
	{
		transformationChangedf();
	}
	
    transformf(const matrix4x4f& tm):
    matrix4x4f(tm)
    {
        transformationChangedf();
    }
    
	virtual ~transformf(){}
	
	const float* getOpenGLMatrix()	{	return m;	}
	
	void updatePositionf(float aXdt, float aYdt, float aZdt)
	{
		m[12]	+= aXdt;
		m[13]	+= aYdt;
		m[14]	+= aZdt;
		
		transformationChangedf();
	}
	
	void updateLocalPositionf(float aXdt, float aYdt, float aZdt)
	{
		if(aXdt!=0)	translateLocalXf(aXdt);
		if(aYdt!=0)	translateLocalYf(aYdt);
		if(aZdt!=0)	translateLocalZf(aZdt);
		
		transformationChangedf();
	}
	
	void translateLocalXf(float aXdt)
	{
		m[12] += (aXdt* m[0]);
		m[13] += (aXdt* m[1]);
		m[14] += (aXdt* m[2]);
	}
	void translateLocalYf(float aYdt)
	{
		m[12] += (aYdt* m[4]);
		m[13] += (aYdt* m[5]);
		m[14] += (aYdt* m[6]);
	}
	void translateLocalZf(float aZdt)
	{
		m[12] += (aZdt* m[8]);
		m[13] += (aZdt* m[9]);
		m[14] += (aZdt* m[10]);
	}
	
    void scalef(float scale)
    {
        matrix4x4f scaleTM;

        scaleTM.setScale(scale, scale, scale);
        //multiply with transform matrix
		matrix4x4f* p=this;
		*p = (scaleTM * (*this));
		
		transformationChangedf();    
    }
	
	void rotateWorldXf(float aAngleInDeg)
	{
		matrix4x4f aRotMat;
		
		aRotMat.setRotationMatrix(aAngleInDeg, true, false, false);
		//multiply with transform matrix
		matrix4x4f* p=this;
		*p = (aRotMat * (*this));
		
		transformationChangedf();
	}
	
	void rotateWorldYf(float aAngleInDeg)
	{
		matrix4x4f aRotMat;
		
		aRotMat.setRotationMatrix(aAngleInDeg, false, true, false);
		//multiply with transform matrix
		matrix4x4f* p=this;
		*p = (aRotMat * (*this));
		
		transformationChangedf();
	}
	
	void rotateWorldZf(float aAngleInDeg)
	{
		matrix4x4f aRotMat;
		
		aRotMat.setRotationMatrix(aAngleInDeg, false, false, true);
		//multiply with transform matrix
		matrix4x4f* p=this;
		*p = (aRotMat * (*this));
		
		transformationChangedf();
	}
	
	void rotateLocalXf(float aAngleInDeg)
	{
		matrix4x4f	aRotMat;
		
		aRotMat.setRotationMatrix(aAngleInDeg, getXAxis());
		vector3f	aTransPoint;
		aTransPoint.x=m[12];
		aTransPoint.y=m[13];
		aTransPoint.z=m[14];
		
		aRotMat.setPosition(-aTransPoint.x, -aTransPoint.y, -aTransPoint.z);
		
		//multiply with transform matrix
		matrix4x4f* p=this;
		*p = (aRotMat * (*this));
		
		m[12]=aTransPoint.x;
		m[13]=aTransPoint.y;
		m[14]=aTransPoint.z;
		
		transformationChangedf();
	}
	
	void rotateLocalYf(float aAngleInDeg)
	{
		matrix4x4f	aRotMat;
		
		aRotMat.setRotationMatrix(aAngleInDeg, getYAxis());
		vector3f	aTransPoint;
		aTransPoint.x=m[12];
		aTransPoint.y=m[13];
		aTransPoint.z=m[14];
		
		aRotMat.setPosition(-aTransPoint.x, -aTransPoint.y, -aTransPoint.z);
		
		//multiply with transform matrix
		matrix4x4f* p=this;
		*p = (aRotMat * (*this));
		
		m[12]=aTransPoint.x;
		m[13]=aTransPoint.y;
		m[14]=aTransPoint.z;
		
		transformationChangedf();
	}
	
	void rotateLocalZf(float aAngleInDeg)
	{
		matrix4x4f	aRotMat;
		
		aRotMat.setRotationMatrix(aAngleInDeg, getZAxis());
		vector3f	aTransPoint;
		aTransPoint.x=m[12];
		aTransPoint.y=m[13];
		aTransPoint.z=m[14];
		
		aRotMat.setPosition(-aTransPoint.x, -aTransPoint.y, -aTransPoint.z);
		
		//multiply with transform matrix
		matrix4x4f* p=this;
		*p = (aRotMat * (*this));
		
		m[12]=aTransPoint.x;
		m[13]=aTransPoint.y;
		m[14]=aTransPoint.z;
		
		transformationChangedf();
	}
	
	void rotateArb(float aAngleInDeg, float* aAxis, vector3f aPos)
	{
		matrix4x4f	aRotMat;
		aRotMat.setRotationMatrix(aAngleInDeg, &aAxis[0]);
		
		vector3f	aTransPoint;
		m[12]-=aPos.x;
		m[13]-=aPos.y;
		m[14]-=aPos.z;
		
		//multiply with transform matrix
		matrix4x4f* p=this;
		*p = (aRotMat * (*this));
		
		m[12]+=aPos.x;
		m[13]+=aPos.y;
		m[14]+=aPos.z;
		
		transformationChangedf();
	}
	
	void setDirection(vector3f* aDir, vector3f* aUp=NULL)
	{
		setDir(aDir, aUp);
		transformationChangedf();
	}
	
	void setPosition(float x, float y, float z)
	{
		m[12]=x;
		m[13]=y;
		m[14]=z;
		transformationChangedf();
	}

	void set2DPosition(float x, float y)
	{
		m[12] = x;
		m[13] = y;
		transformationChangedf();
	}
    
    void setPosition(const vector2f& pos)
    {
        m[12]=pos.x;
        m[13]=pos.y;
        transformationChangedf();
    }
	
	void setPosition(const vector3f& pos)
    {
        m[12]=pos.x;
        m[13]=pos.y;
        m[14]=pos.z;
        transformationChangedf();
    }

	virtual void transformationChangedf()
    {
        
    }
};

#endif