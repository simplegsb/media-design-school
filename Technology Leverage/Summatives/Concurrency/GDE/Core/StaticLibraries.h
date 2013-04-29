/**
 *	@file GDE\GDE\Core\StaticLibraries.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _STATICLIBRARIES_H__Included_1205251915
#define _STATICLIBRARIES_H__Included_1205251915

namespace GDE
{
	/**
	 *	@ingroup Core.
	 *
	 *	Singleton class to register and recieve requests to create new libraries.
	 */
	class CStaticLibraryManager
	{
	public:
		typedef void* (cbCreateLib)(void);	//!< callback function to create the library.
		typedef void (cbKillLib)(void*);	//!< callback function to delete the library.

		static CStaticLibraryManager& GetInstance( void );

		void*	GetInterface( const char* strName,  int iDLL_ID, cbKillLib*& pKillLib );
		bool	RegisterLibrary( cbCreateLib* pCreateFN, cbKillLib* pKillFN, const char* strName, int iDLL_ID );

	protected:
		CStaticLibraryManager( void );	// singleton!
		~CStaticLibraryManager( void );

		struct SLibEntry
		{
			cbCreateLib*	m_pCreateFN;	//!< Creation function.
			cbKillLib*		m_pKillFN;		//!< Destruction function.
			int				m_iDLL_ID;		//!< DLL id
			const char*		m_strName;		//!< Library name;
			SLibEntry*		m_pNext;		//!< These are a list.
		};

		SLibEntry*		GetLibrary( const char* strName, int iDLL_ID );

		SLibEntry*		m_pLibraries;	//!< List of all libraries in the manager.
	};

	/**
	 *	Class used to register static libraries.
	 */
	class CStaticLibraryRegistrar
	{
	public:
		CStaticLibraryRegistrar( CStaticLibraryManager::cbCreateLib* pCreateFN,
			CStaticLibraryManager::cbKillLib* pKillFN,
			const char* strName, char cID_0, char cID_1, char cID_2, char cID_3 );		
	};

	// macro used to create a thing.
#define _REGISTER_STATIC_LIB( pCreateFN, pKillFN, strName, cID0, cID1, cID2, cID3 )	static GDE::CStaticLibraryRegistrar gs_LibRegistrar##__LINE__( (pCreateFN), (pKillFN), (strName), cID0, cID1, cID2, cID3 )
}; // end namespace GDE.

#endif //_STATICLIBRARIES_H__Included_1205251915

