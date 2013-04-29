/**
 *	@file GDE\DataParser\DataParser.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "GDE/Core/Core.h"
#include "DataParser.h"
#include "DataParserImp.h"

using namespace GDE;

/**
 *	@param uValue		Value to be filled in - left alone on fail.
 *	@param strSection	Section name to lookup in.
 *	@param strId		Id to be looked up..
 *	@return				Success.
 */
bool	CDataParser::GetValue( UINT32& uValue, const char* strSection, const char* strId )
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->GetValue( uValue, strSection, strId );
}
/**
 *	@param uValue		Value to set.
 *	@param strSection	Section name to set in.
 *	@param strId		Id to be set.
 *	@return				Success.
 */
bool	CDataParser::SetValue( UINT32 uValue, const char* strSection, const char* strId )
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->SetValue( uValue, strSection, strId );
}
/**
 *	@param iValue		Value to be filled in - left alone on fail.
 *	@param strSection	Section name to lookup in.
 *	@param strId		Id to be looked up..
 *	@return				Success.
 */
bool	CDataParser::GetValue( SINT32& iValue, const char* strSection, const char* strId )
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->GetValue( iValue, strSection, strId );
}
/**
 *	@param iValue		Value to set.
 *	@param strSection	Section name to set in.
 *	@param strId		Id to be set.
 *	@return				Success.
 */
bool	CDataParser::SetValue( SINT32 iValue, const char* strSection, const char* strId )
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->SetValue( iValue, strSection, strId );
}
/**
 *	@param fValue		Value to be filled in - left alone on fail.
 *	@param strSection	Section name to lookup in.
 *	@param strId		Id to be looked up..
 *	@return				Success.
 */
bool	CDataParser::GetValue( FLOAT32& fValue, const char* strSection, const char* strId )
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->GetValue( fValue, strSection, strId );
}
/**
 *	@param fValue		Value to set.
 *	@param strSection	Section name to set in.
 *	@param strId		Id to be set.
 *	@return				Success.
 */
bool	CDataParser::SetValue( FLOAT32 fValue, const char* strSection, const char* strId )
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->SetValue( fValue, strSection, strId );
}

/**
 *	@param strValue		Value to be filled in - left alone on fail, unless uMaxChars is met.
 *	@param uMaxChars	The maximum number of characters to fill in. Will return fail if exceeded.
 *	@param strSection	Section name to lookup in.
 *	@param strId		Id to be looked up..
 *	@return				Success.
 */
bool	CDataParser::GetString( char* strValue, const UINT32 uMaxChars, const char* strSection, const char* strId )
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->GetString( strValue, uMaxChars, strSection, strId );
}

/**
 *	@param strValue		Value to set.
 *	@param strSection	Section name to set in.
 *	@param strId		Id to be set.
 *	@return				Success.
 */
bool	CDataParser::SetString( const char* strValue, const char* strSection, const char* strId )
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->SetString( strValue, strSection, strId );
}
