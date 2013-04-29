/**
 *	@file GDE\Core\Observer.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _OBSERVER_H__Included_1658944758
#define _OBSERVER_H__Included_1658944758

namespace GDE
{
	// so this class will help us to build up an observer/subject or
	// listener/subject relationship
	class IObserver;
	/**
	 *	@ingroup Core
	 *
	 *	An observer subject - something that can be observed.
	 *	@see IObserver.
	 */
	class ISubject
	{
	public:
		virtual ~ISubject( void ) {} // ensure all destructors are virtual.
		virtual bool AddObserver( IObserver* pObserver, UINT32 uMessage ) = 0;
		virtual bool RemoveObserver( IObserver* pObserver, UINT32 uMessage ) = 0;
	};

	/**
	 *	@ingroup Core
	 *
	 *	An observer that can watch and be notified of state changes to subjects.
	 *	@see ISubject.
	 */
	class IObserver
	{
	public:
		virtual ~IObserver( void ) {};
		/**
		 *	The subject will call this when it is required.
		 *	Our observer/listener has to implement this function
		 */
		virtual void NotifyChange( ISubject* pSubject, UINT32 uMessage ) = 0;
	protected:
	};

}	// end namespace GDE

#endif //_OBSERVER_H__Included_1658944758

