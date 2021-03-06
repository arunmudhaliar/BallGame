#include "gxFile.h"
#include "objC_util.h"
#include <errno.h>

gxFile::gxFile()
{
	m_eFileMode=FILE_r;
	m_pFP=NULL;
}

gxFile::~gxFile()
{
	CloseFile();
}

int gxFile::OpenFile(const char* pszFileName, EFILEMODE eFileMode/* =FILE_r */)
{
	//int err=0;
	m_eFileMode=eFileMode;
	
	switch(m_eFileMode)
	{
		case FILE_r: m_pFP = fopen(objC_util::getPath(pszFileName), "rb");		break;
		case FILE_w: m_pFP = fopen(objC_util::getPath(pszFileName), "wb");		break;
		case FILE_a: m_pFP = fopen(objC_util::getPath(pszFileName), "a");		break;
	}

	if(m_pFP==NULL)
    {        
#if defined (LOG_DEBUG_ENGINE)
        DEBUG_PRINT("Error opening file - %s, %s", pszFileName, strerror(errno));
#endif 
        
    }
	
	return (m_pFP)?1:0;
}

int gxFile::OpenFileDescriptor(int fd, EFILEMODE eFileMode)
{
	//int err=0;
	m_eFileMode=eFileMode;
	
	switch(m_eFileMode)
	{
		case FILE_r: m_pFP = fdopen(fd, "rb");		break;
		case FILE_w: m_pFP = fdopen(fd, "wb");		break;
		case FILE_a: m_pFP = fdopen(fd, "a");		break;
	}

    if(m_pFP==NULL)
    {        
#if defined (LOG_DEBUG_ENGINE)
        DEBUG_PRINT("Error opening file - %s", strerror(errno));
#endif 
    }
	return (m_pFP)?1:0;
}

void gxFile::Seek(unsigned int off, int flag) const
{
	fseek(m_pFP, off, flag);
}

long gxFile::Tell() const
{
    return ftell(m_pFP);
}

void gxFile::CloseFile()
{
	if(m_pFP)
    {
//#ifndef ANDROID
        if(fclose(m_pFP))
        {
#if defined (LOG_DEBUG_ENGINE)
            DEBUG_PRINT("Error closing file - %s", strerror(errno));
#endif 
            
        }
//#endif
    }
	m_pFP=NULL;
}

void gxFile::Read(char& v) const
{
	fread((void*)&v, sizeof(char), 1, m_pFP);
}

char* gxFile::ReadString() const
{
	char* aVar=NULL;
	int aCount;
	Read(aCount);
	aVar=new char[aCount+1];	
	fread(aVar, aCount, 1, m_pFP);
	aVar[aCount]='\0';						//This is a must

	return aVar;
}

void gxFile::Read(byte& v) const
{
	fread((void*)&v, sizeof(byte), 1, m_pFP);
}

void gxFile::Read(bool& v) const
{
	fread((void*)&v, sizeof(bool), 1, m_pFP);
}

void gxFile::Read(short& v) const
{
	fread((void*)&v, sizeof(short), 1, m_pFP);
}

void gxFile::Read(unsigned short& v) const
{
	fread((void*)&v, sizeof(unsigned short), 1, m_pFP);
}

void gxFile::Read(int& v) const
{
	fread((void*)&v, sizeof(int), 1, m_pFP);
}

void gxFile::Read(unsigned int& v) const
{
	fread((void*)&v, sizeof(unsigned int), 1, m_pFP);
}

void gxFile::Read(float& v) const
{
	fread((void*)&v, sizeof(float), 1, m_pFP);
}

void gxFile::Read(double& v) const
{
	fread((void*)&v, sizeof(double), 1, m_pFP);
}

void gxFile::Read(unsigned long& v) const
{
	fread((void*)&v, sizeof(unsigned long), 1, m_pFP);    
}

void gxFile::ReadBuffer(unsigned char* buffer, unsigned long cnt) const
{
    fread((void*)buffer, cnt, 1, m_pFP);
}

//----------------Write Functions-----------------
void gxFile::Write(char v) const
{
	Write(&v, sizeof(char));
}

void gxFile::Write(const char		aVar[]) const
{
	int aCount;
	aCount=(int)strlen(aVar);					//Get the length of the String
	Write(aCount);							//Write the string length on to the stream
	Write(aVar,aCount);						//Write the String on to the stream
}

void gxFile::Write(byte v) const
{
	Write(&v, sizeof(byte));
}

void gxFile::Write(bool v) const
{
	Write(&v, sizeof(bool));
}

void gxFile::Write(short v) const
{
	Write(&v, sizeof(short));
}

void gxFile::Write(unsigned short v) const
{
	Write(&v, sizeof(unsigned short));
}

void gxFile::Write(int v) const
{
	Write(&v, sizeof(int));
}

void gxFile::Write(unsigned int v) const
{
	Write(&v, sizeof(unsigned int));
}

void gxFile::Write(float v) const
{
	Write(&v, sizeof(float));
}

void gxFile::Write(double v) const
{
	Write(&v, sizeof(double));
}

void gxFile::Write(const void* pAny, int iCount) const
{
	fwrite((char*)pAny, iCount, 1, m_pFP);
}