#ifndef FILEREADER_H
#define FILEREADER_H

#include <fstream>

class CFileReader;
class CFile;

class CFileReader
{
public:
	CFileReader();
	~CFileReader();

	bool ReadFile( const char *sFilePath, int iFlags );
	CFile *GetFile( void ) const;

private:
	CFile *m_pFile;
};

class CFile
{
public:
	CFile( const char *sFilePath, int iFlags );
	~CFile();

	bool Success( void ) const;

	bool Buffer( char *&sBuffer ) const;

private:
	bool Write( void *pData, unsigned int uiSize ) const;
	bool Read( void *pData, unsigned int uiSize ) const;

private:
	std::fstream *m_pFileStream;
};

#endif // FILEREADER_H