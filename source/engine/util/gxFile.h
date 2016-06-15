#ifndef GXFILE_H
#define GXFILE_H

#include "../core/types.h"
//#include <fstream>

class gxFile
{
public:
	enum EFILEMODE
	{
		FILE_r,
		FILE_w,
		FILE_a
	};

	gxFile();
	~gxFile();

	int	OpenFile(const char* pszFileName, EFILEMODE eFileMode=FILE_r);
	int OpenFileDescriptor(int fd, EFILEMODE eFileMode=FILE_r);
	//int OpenFile(FILE* fp);
	
	void	Seek(unsigned int off, int flag) const;
	long    Tell() const;
    
	void	CloseFile();

	void	Read(char& v) const;
	char* ReadString() const;
	void	Read(byte& v) const;
	void	Read(bool& v) const;
	void	Read(short& v) const;
	void	Read(unsigned short& v) const;
	void	Read(int& v) const;
	void	Read(unsigned int& v) const;
	void	Read(float& v) const;
	void	Read(double& v) const;
    void    Read(unsigned long& v) const;
    void    ReadBuffer(unsigned char* buffer, unsigned long cnt) const;

	void	Write(char v) const;
	void	Write(const char aVar[]) const;
	void	Write(byte v) const;
	void	Write(bool v) const;
	void	Write(short v) const;
	void	Write(unsigned short v) const;
	void	Write(int v) const;
	void	Write(unsigned int v) const;
	void	Write(float v) const;
	void	Write(double v) const;

private:
	void	Write(const void* pAny, int iCount) const;

	FILE*		m_pFP;
	EFILEMODE	m_eFileMode;
};

#endif