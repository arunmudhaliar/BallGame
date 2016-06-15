#ifndef PLAYERDATA_H
#define PLAYERDATA_H

#include "../engine/util/gxFile.h"

class playerData
{
public:
	playerData()
	{
		resetScores();
		m_iHighScore = 0;
	}

	~playerData()
	{
	}

	bool readFromPref()
	{
		gxFile file;
		if(!file.OpenFile("data.bin"))
		{
			return false;
		}

		file.Read(m_iHighScore);
		file.CloseFile();

		return true;
	}

	bool writeToPref()
	{
		gxFile file;
		if(!file.OpenFile("data.bin", gxFile::FILE_w))
		{
			return false;
		}

		file.Write(m_iHighScore);
		file.CloseFile();

		return true;
	}

	void resetScores()
	{
		m_iCurrentPlayingLevel=0;
		m_iCurrentScore=0;
		m_iCurrentAsteroidDestroyed=0;
	}

//private:
	int m_iCurrentPlayingLevel;
	int m_iCurrentScore;

	int m_iCurrentAsteroidDestroyed;
	int m_iHighScore;
};

#endif