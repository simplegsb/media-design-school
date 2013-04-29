/**
 *	@file GDE\DataParser\DataParser.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _DATAPARSER_H__Included_768753273
#define _DATAPARSER_H__Included_768753273

#include "GDE/Core/ResourceBridge.h"

namespace GDE
{

class IDataParserImp;
class CDataParserManager;
class IDataParserManagerImp;

/**
 *	The Data Parser class.
 */
class CDataParser : public TResourceBridge< CDataParser, CDataParserManager, IDataParserImp, IDataParserManagerImp, CREATE_DLL_ID( 'D','P','R','S') >
{
public:
	typedef CDataParser TThis;
	typedef IDataParserImp TImplementer;
	typedef TResourceBridge< CDataParser, CDataParserManager, IDataParserImp, IDataParserManagerImp, CREATE_DLL_ID( 'D','P','R','S') > TParent;
	typedef TResourceManagerBridge< CDataParserManager, IDataParserManagerImp, TThis, CREATE_DLL_ID( 'D','P','R','S') > TManagerBase;

	bool	GetValue( UINT32& uValue, const char* strSection, const char* strId );
	bool	SetValue( UINT32 uValue, const char* strSection, const char* strId );
	bool	GetValue( SINT32& iValue, const char* strSection, const char* strId );
	bool	SetValue( SINT32 iValue, const char* strSection, const char* strId );
	bool	GetValue( FLOAT32& iValue, const char* strSection, const char* strId );
	bool	SetValue( FLOAT32 iValue, const char* strSection, const char* strId );
	bool	GetString( char* strValue, const UINT32 uMaxChars, const char* strSection, const char* strId );
	bool	SetString( const char* strValue, const char* strSection, const char* strId );
protected:
	// need to be a special friend to allow the manager access to create these:
	friend class CDataParserManager;
	friend class TManagerBase;

	CDataParser( IDataParserImp* pImplementer, const CResourceId& Id, CDataParserManager* pManager, CDataParser* pNext )
	: TParent( pImplementer, Id, pManager, pNext ) 
	{	// empty
	}
};

} // end namespace GDE

#endif //_DATAPARSER_H__Included_768753273

