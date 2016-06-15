#ifndef GXSHADER_H
#define GXSHADER_H

class gxShader
{
public:
	gxShader()
	{}
	virtual ~gxShader()
	{}
    
   	virtual int getAttribLoc(const char* uvar)
    {
        return -1;
    }
};

#endif