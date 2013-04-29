/**
 *	@file GDE\Core\Core.cpp
 *	@brief For now, this file simply contains some documentation for doxygen.
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
namespace GDE // This is simply to make doxygen pay attention to the namespace!
{
/**
 *	@mainpage
 *	
 *	Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *
 *	@section Overview GDE Overview
 *	GDE or Game Developlment Engine is an engine designed for use in
 *	Media Design School's Graduate Diploma of Game Development.
 *	If you are not an MDS tutor or student then you should not have this!
 *
 *	@section DesignPhilosophy Design Philosopy
 *
 *	Wing it - no, there is a philosopy, it just needs describing/
 *	One of the main design philosophies is to use the bridge design pattern
 *	to decouple interface from implementations and extend that so that
 *	custom implementations can be loaded as DLLs. This is facilitated by the
 *	TBridge template, that is further subclassed (using the curiously recurring
 *  template pattern) by interfaces that support this.
 *
 *	It seems that there are two common types of interfaces that are encountered,
 *	each subclassed from a bridge, the resource and resource manager, TResourceBridge
 *	and TResourceManagerBridge respectively. Resource managers facilitate aquiring and
 *	maintaining a number of resources. The resource manager bridge will
 *	maintain a list of all the resources which will only be released from memory
 *	when they are fully released. @see TResourceBridge::AddRef and TResourceBridge#Release
 */
/**
 *	@defgroup Core Core Library
 *	The core functionality is required by everything. Nice. 
 */
}
