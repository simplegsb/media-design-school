/**
 *	@file GDE\Renderer\SpriteAnim.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "GDE/Core/Core.h"
#include "SpriteAnim.h"
#include "SpriteAnimImp.h"

namespace GDE
{

/**
 *	Play or resume playing an animation.
 *
 *	@return Success.
 */
bool	CSpriteAnim::Play( void )
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->Play();
}
/**
 *	@return Is the animation playing, or false on failure.
 */
bool	CSpriteAnim::IsPlaying( void ) const
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->IsPlaying();
}
/**
 *	Stop, or pause a playing animation.
 *
 *	@return Success.
 */
bool	CSpriteAnim::Stop( void )
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->Stop();
}
/**
 *	Reset the animation to the start.
 *
 *	@return Success.
 */
bool	CSpriteAnim::Reset( void )
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->Reset();
}
/**
 *	Set the animation to looping.
 *
 *	@return Success.
 */
bool	CSpriteAnim::SetLooping( bool bValue )
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->SetLooping( bValue );
}
/**
 *	@return Is the animation looping, or false on failure.
 *
 *	@return The previous looping state or false on error.
 */
bool	CSpriteAnim::IsLooping( void ) const
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->IsLooping();
}
/**
 *	Set the animation to playing backwards.
 *	@return The previous state or false on error.
 */
bool	CSpriteAnim::SetReverse( bool bValue )
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->SetReverse( bValue );
}
/**
 *	@return Is the animation playing in reverse, or false on failure.
 */
bool	CSpriteAnim::IsReverse( void ) const
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->IsReverse();
}
/**
 *	Set the animation to ping pong - play once forwards and then reverse back through.
 *	@return the previous state or false on error.
 */
bool	CSpriteAnim::SetPingPong( bool bValue )
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->SetPingPong( bValue );
}
/**
 *	@return Is the animation doing ping-ping cycles, or false on failure.
 */
bool	CSpriteAnim::IsPingPong( void ) const
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->IsPingPong();
}
/**
 *	Set the current animation playback speed, 1.0f for normal operation.
 *
 *	@return Success.
 */
bool	CSpriteAnim::SetSpeed( FLOAT32 fSpeed )
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->SetSpeed( fSpeed );
}
/**
 *	@return The current animation playback speed, 1.0f for normal operation.
 */
FLOAT32	CSpriteAnim::GetSpeed( void ) const
{
	if ( NULL == m_pImplementer )
	{
		return 0.0f;
	}
	return m_pImplementer->GetSpeed();
}
/**
 *	@return the current frame number.
 */
UINT32	CSpriteAnim::GetFrame( void ) const
{
	if ( NULL == m_pImplementer )
	{
		return 0;
	}
	return m_pImplementer->GetFrame();
}
/**
 *	@return the number of frames.
 */
UINT32	CSpriteAnim::GetFrameCount( void ) const
{
	if ( NULL == m_pImplementer )
	{
		return 0;
	}
	return m_pImplementer->GetFrameCount();
}
/**
 *	Update the animation
 *	@param fTimeDelta The time difference to update over.
 *
 *	@return Success.
 */
bool	CSpriteAnim::Update( FLOAT32 fTimeDelta )
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->Update( fTimeDelta );
}
/**
 *	@return The sprite this animation modifies, or NULL on failure.
 */
CSprite* CSpriteAnim::GetSprite( void )
{
	if ( NULL == m_pImplementer )
	{
		return NULL;
	}
	return m_pImplementer->GetSprite();
}

} // end namespace GDE

