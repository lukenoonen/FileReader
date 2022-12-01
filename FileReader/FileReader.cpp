//=============== Copyright Luke Noonen, All rights reserved. ===============//
//
// Purpose: Defines the CFileReader class and its component class, CFile
//
//===========================================================================//

#include "FileReader.h"
#include <string>

//-----------------------------------------------------------------------------
// Purpose: 
// Output: true on successful write of string, false otherwise
//-----------------------------------------------------------------------------
bool UTIL_Write( CFile *pFile, char *&sData )
{
	unsigned int uiSize = (unsigned int)strlen( sData );
	if (!UTIL_Write( pFile, uiSize ))
		return false;

	if (!pFile->WriteBytes( sData, sizeof( char ) * uiSize ))
		return false;

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output: true on successful read of string, false otherwise
// Note: dynamically allocates memory for string, be sure to delete!
//-----------------------------------------------------------------------------
bool UTIL_Read( CFile *pFile, char *&sData )
{
	unsigned int uiSize;
	if (!UTIL_Read( pFile, uiSize ))
		return false;

	sData = new char[uiSize + 1];
	if (!pFile->ReadBytes( sData, sizeof( char ) * uiSize ))
	{
		delete[] sData;
		return false;
	}

	sData[uiSize] = '\0';
	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CFileReader::CFileReader()
{
	m_pFile = NULL;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CFileReader::~CFileReader()
{
	if (m_pFile)
		delete m_pFile;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output: true upon successful read of file, false otherwise
//-----------------------------------------------------------------------------
bool CFileReader::ReadFile( const char *sFilePath, int iFlags )
{
	if (m_pFile)
		delete m_pFile;

	m_pFile = new CFile( sFilePath, iFlags );

	if (!m_pFile->Success())
	{
		delete m_pFile;
		m_pFile = NULL;
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output: the CFile instance
// Note: If read file has failed, will return NULL
//-----------------------------------------------------------------------------
CFile *CFileReader::GetFile( void ) const
{
	return m_pFile;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CFile::CFile( const char *sFilePath, int iFlags )
{
	m_fFileStream.open( sFilePath, iFlags );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CFile::~CFile()
{
	if (m_fFileStream.is_open())
		m_fFileStream.close();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output: true on successful file read, false otherwise
//-----------------------------------------------------------------------------
bool CFile::Success( void ) const
{
	return m_fFileStream.is_open();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output: true on successful file buffer, false otherwise
//-----------------------------------------------------------------------------
bool CFile::Buffer( char *&sBuffer )
{
	std::streampos spInitialPosition = m_fFileStream.tellg();
	m_fFileStream.ignore( std::numeric_limits<std::streamsize>::max() );
	unsigned int uiSize = (unsigned int)m_fFileStream.gcount();
	m_fFileStream.clear();
	m_fFileStream.seekg( spInitialPosition, std::ios_base::beg );

	sBuffer = new char[uiSize + 1];
	if (!ReadBytes( sBuffer, sizeof( char ) * uiSize ))
	{
		delete[] sBuffer;
		return false;
	}

	sBuffer[uiSize] = '\0';
	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output: true on successful file write, false otherwise
//-----------------------------------------------------------------------------
bool CFile::WriteBytes( void *pData, unsigned int uiSize )
{
	return m_fFileStream.write( (char *)(pData), uiSize ) ? true : false; // Maybe perform checks to see if the file can be written to
}

//-----------------------------------------------------------------------------
// Purpose: true on successful file read, false otherwise
//-----------------------------------------------------------------------------
bool CFile::ReadBytes( void *pData, unsigned int uiSize )
{
	return m_fFileStream.read( (char *)(pData), uiSize ) ? true : false; // Maybe perform checks to see if the file can be read from
}