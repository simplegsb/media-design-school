//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// Copyright (c) 2012 Media Design School
//
// SVN Info
// $Author: MEDIADESIGN\gary.buyn $
// $Date: 2013-01-23 09:43:23 +1300 (Wed, 23 Jan 2013) $
// $Rev: 1109 $
//
#pragma once

#include <windows.h>

#include <Box2D/Box2D.h>

class CBox2DGDIDrawer : public b2Draw
{
	public:
		CBox2DGDIDrawer(HWND window);

		~CBox2DGDIDrawer();

		void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

		void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
		
		void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
	
		void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
	
		void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);

		void DrawTransform(const b2Transform& xf);

		void EndFrame();

		void SetupFrame();

	private:
		HBITMAP backBitmap;

		HBITMAP backBitmapOld;

		HDC backBuffer;

		HDC frontBuffer;

		int height;

		int width;

		HWND window;
};
