/**
 *	@file GDE\Core\ResourceBridge.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _RESOURCEBRIDGE_H__Included_310043916
#define _RESOURCEBRIDGE_H__Included_310043916

#include "GDE/Core/Bridge.h"
#include "GDE/Core/ResourceId.h"

namespace GDE
{
	// forward reference a template?
	template <typename TManagerType, typename TImplementer, typename TResourceBridge, int iDLL_ID> class TResourceManagerBridge;

	/**
	 *	@ingroup Core
	 *
	 *	This template extends the bridge, specifically extended for common functionality required by resources.
	 *	So reference counted etc.
	 *	The DLL ID should simply be the same one as is used for the manager that is responsible for the resource.
	 *	@see IResource.
	 *
	 *	@tparam TResourceType		The final resource type, (what inherits from this!)
	 *	@tparam TManagerType		The manager that manages this.
	 *	@tparam	TImplementer		The implementor interface for the bridge.
	 *	@tparam	TManagerImplementer	The implementor for the manager.
	 */
	template < typename TResourceType, typename TManagerType, typename TImplementer, typename TManagerImplementer, int iDLL_ID>
	class TResourceBridge : public TBridge< TImplementer, iDLL_ID>
	{
	public:
		typedef TBridge< TImplementer, iDLL_ID> TParent;
		typedef TResourceBridge<TResourceType, TManagerType, TImplementer, TManagerImplementer,  iDLL_ID> TThis;
		typedef TResourceManagerBridge<TManagerType, TManagerImplementer, TResourceType, iDLL_ID> TManagerBase;

		void				AddRef( void );	
		bool				Release( void );
		UINT32				GetRefCount( void ) const;
		const CResourceId&	GetId( void ) const;
	protected:
		friend class TResourceManagerBridge<TManagerType, TManagerImplementer, TResourceType, iDLL_ID>;

		/**
		 *	Constructor - only used by the manager, needs explicitly recreating for each child in a protected manner.
		 */
		TResourceBridge( TImplementer* pImplementer, const CResourceId& Id, TManagerType* pManager, TResourceType* pNext )
		: TParent( pImplementer )
		, m_uRefCount( 1 )
		, m_pNext( pNext )
		, m_pManager( pManager )
		, m_Id( Id )
		{	// empty.
		}

		UINT32					m_uRefCount;	//!< Current reference count.
		TResourceType*			m_pNext;		//!< The next resource in the linked list of all resources of this type.
		TManagerType*			m_pManager;		//!< The manager that owns this.
		CResourceId				m_Id;			//!< The Id of this resource.

	private:
		// hide away the following parent class implementation as it is inappropriate.
		using TParent::LoadDLL;
		using TParent::ReleaseDLLImplementer;

	};

	////////////////////////////////////////////////////
	// inline implementation of the above:

	/**
	 *	Add a reference to this resource.
	 */
	template <typename TResourceType,typename TManagerType,typename TImplementer, typename TManagerImplementer, int iDLL_ID>
	void TResourceBridge<TResourceType,TManagerType,TImplementer,TManagerImplementer,iDLL_ID>::AddRef( void )
	{
		++m_uRefCount;
	}

	/**
	 *	Release this resource.
	 *
	 *	@return true if this was the last reference count freed and so the resource is finished with, or if the bridge is not implemented.
	 */
	template <typename TResourceType,typename TManagerType, typename TImplementer, typename TManagerImplementer, int iDLL_ID>
	bool TResourceBridge<TResourceType,TManagerType,TImplementer,TManagerImplementer,iDLL_ID>::Release( void )
	{
		--m_uRefCount;
		if ( 0 == m_uRefCount )
		{
			m_pManager->ReleaseResource( reinterpret_cast<TResourceType*>(this) );
			return true;
		}
		return false;
	}

	/**
	 *	@return the current reference count for the resource or zero if the bridge is not implemented.
	 */
	template <typename TResourceType,typename TManagerType, typename TImplementer, typename TManagerImplementer, int iDLL_ID>
	UINT32 TResourceBridge<TResourceType,TManagerType,TImplementer,TManagerImplementer,iDLL_ID>::GetRefCount( void ) const
	{
		return m_uRefCount;
	}

	/**
	 *	@return The resource Id of this resouce
	 */
	template <typename TResourceType,typename TManagerType, typename TImplementer, typename TManagerImplementer, int iDLL_ID>
	const CResourceId&	TResourceBridge<TResourceType,TManagerType,TImplementer,TManagerImplementer,iDLL_ID>::GetId( void ) const
	{
		return m_Id;
	}


};
#endif //_RESOURCEBRIDGE_H__Included_310043916

