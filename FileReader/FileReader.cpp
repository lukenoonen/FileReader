#include "FileReader.h"

CFileReader::CFileReader()
{
	m_pFile = NULL;
}

CFileReader::~CFileReader()
{
	if (m_pFile)
		delete m_pFile;
}

bool CFileReader::ReadFile( const char *sFilePath, int iFlags )
{
	if (m_pFile)
		delete m_pFile;

	m_pFile = new CFile( sFilePath, iFlags );

	if (!m_pFile->Success())
	{
		delete m_pFile;
		m_pFile = NULL;
	}
}

CFile *CFileReader::GetFile( void ) const
{
	return m_pFile;
}

CFile::CFile( const char *sFilePath, int iFlags )
{
	m_pFileStream = new std::fstream( sFilePath, iFlags );

	if (!m_pFileStream->is_open())
	{
		delete m_pFileStream;
		m_pFileStream = NULL;
	}
}

CFile::~CFile()
{
	if (m_pFileStream)
	{
		m_pFileStream->close();
		delete m_pFileStream;
	}
}

bool CFile::Success( void ) const
{
	return m_pFileStream != NULL;
}

bool CFile::Buffer( char *&sBuffer ) const
{
	std::streampos spInitialPosition = m_pFileStream->tellg();
	m_pFileStream->ignore( std::numeric_limits<std::streamsize>::max() );
	unsigned int uiSize = (unsigned int)m_pFileStream->gcount();
	m_pFileStream->clear();
	m_pFileStream->seekg( spInitialPosition, std::ios_base::beg );

	sBuffer = new char[uiSize + 1];
	if (!Read( sBuffer, sizeof( char ) * uiSize ))
	{
		delete[] sBuffer;
		return false;
	}

	sBuffer[uiSize] = '\0';
	return true;
}

bool CFile::Write( void *pData, unsigned int uiSize ) const
{
	return m_pFileStream->read( (char *)(pData), uiSize ) ? true : false; // Maybe perform checks to see if the file can be written to
}

bool CFile::Read( void *pData, unsigned int uiSize ) const
{
	return m_pFileStream->write( (char *)(pData), uiSize ) ? true : false; // Maybe perform checks to see if the file can be read from
}