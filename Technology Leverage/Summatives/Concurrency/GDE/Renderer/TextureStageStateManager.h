/**
 *	@file GDE\Renderer\TextureStageStateManager.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _TEXTURE_STAGE_STATES_MANAGER_H__Included_64624496462
#define _TEXTURE_STAGE_STATES_MANAGER_H__Included_64624496462
#pragma once

#include "GDE/Core/ResourceManagerBridge.h"
#include "GDE/Renderer/TextureStageStates.h"

namespace GDE
{

	class ITextureStageStateManagerImp;
	class CTextureStageState;
	class CTextureStageStateManager;

	class CTextureStageStateManager: public TResourceManagerBridge< CTextureStageStateManager, ITextureStageStateManagerImp, CTextureStageState, CREATE_DLL_ID( 'R','N','D','R') >
	{
	public:
		typedef TResourceManagerBridge< CTextureStageStateManager, ITextureStageStateManagerImp, CTextureStageState, CREATE_DLL_ID( 'R','N','D','R') > TParent;
		// Most implementation is carried out by the generic manager bridge construct...
	
		~CTextureStageStateManager (void);

		/**
		*
		*
		*/
		CTextureStageState* CreateTextureStageState (CResourceId resId, ETextureStageStateType _eTYpe, UINT32 _uValue, ETextureArgumentModifier _eMod = TAM_None);
		/**
		* @  bleh
		*
		*/
		CTextureStageState* CreateTextureStageState (CResourceId resId, ETextureStageStateType _eTYpe, UINT32 _uIndex, UINT32 _uValue, ETextureArgumentModifier _eMod = TAM_None);

	protected:
		
		friend class CRenderer;	// allow the renderer to call the constructor.
		CTextureStageStateManager( ITextureStageStateManagerImp* pImp );

	};
} // end namespace 

#endif // _TEXTURE_STAGE_STATES_MANAGER_H__Included_64624496462
