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
// $Date: 2013-01-29 11:58:25 +1300 (Tue, 29 Jan 2013) $
// $Rev: 1129 $
//
#include "Box2DGDIDrawer.h"

CBox2DGDIDrawer::CBox2DGDIDrawer(HWND window) :
	window(window)
{
	RECT windowRect;
	GetClientRect(window, &windowRect);

	frontBuffer = GetDC(window);
	backBuffer = CreateCompatibleDC(frontBuffer);
	backBitmap = CreateCompatibleBitmap(frontBuffer, windowRect.right, windowRect.bottom);
	backBitmapOld = (HBITMAP) SelectObject(backBuffer, backBitmap);
}

CBox2DGDIDrawer::~CBox2DGDIDrawer()
{
	SelectObject(backBuffer, backBitmapOld);
	DeleteObject(backBitmap);
	ReleaseDC(window, backBuffer);
	ReleaseDC(window, frontBuffer);
}

void CBox2DGDIDrawer::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(color.r * 255, color.g * 255, color.b * 255));
	SelectObject(backBuffer, pen);

	MoveToEx(backBuffer, (int) vertices[0].x, (int) vertices[0].y, NULL);

	for (int32 index = 1; index < vertexCount; index++)
	{
		LineTo(backBuffer, (int) vertices[index].x, (int) vertices[index].y);
	}

	LineTo(backBuffer, (int) vertices[0].x, (int) vertices[0].y);

	DeleteObject(pen);
}

void CBox2DGDIDrawer::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	HBRUSH brush = CreateSolidBrush(RGB(color.r * 255, color.g * 255, color.b * 255));
	SelectObject(backBuffer, brush);

	POINT* points = new POINT[vertexCount];

	for (int32 index = 0; index < vertexCount; index++)
	{
		points[index].x = static_cast<LONG>(vertices[index].x);
		points[index].y = static_cast<LONG>(vertices[index].y);
	}

	Polygon(backBuffer, points, vertexCount);

	delete points;

	DeleteObject(brush);
}

void CBox2DGDIDrawer::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(color.r * 255, color.g * 255, color.b * 255));
	SelectObject(backBuffer, pen);

	Ellipse(backBuffer,
		static_cast<int>(center.x - radius),
		static_cast<int>(center.y - radius),
		static_cast<int>(center.x + radius),
		static_cast<int>(center.y + radius));

	DeleteObject(pen);
}

void CBox2DGDIDrawer::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	HBRUSH brush = CreateSolidBrush(RGB(color.r * 255, color.g * 255, color.b * 255));
	SelectObject(backBuffer, brush);

	Ellipse(backBuffer,
		static_cast<int>(center.x - radius),
		static_cast<int>(center.y - radius),
		static_cast<int>(center.x + radius),
		static_cast<int>(center.y + radius));

	MoveToEx(backBuffer, (int) center.x, (int) center.y, NULL);
	LineTo(backBuffer, (int) center.x + (axis.x * radius), (int) center.y + (axis.y * radius));

	DeleteObject(brush);
}

void CBox2DGDIDrawer::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(color.r * 255, color.g * 255, color.b * 255));
	SelectObject(backBuffer, pen);

	MoveToEx(backBuffer, (int) p1.x, (int) p1.y, NULL);
	LineTo(backBuffer, (int) p2.x, (int) p2.y);

	DeleteObject(pen);
}

void CBox2DGDIDrawer::DrawTransform(const b2Transform& xf)
{
	//DrawCircle(xf.p, 1.0f, b2Color(0.0f, 0.0f, 0.0f));
}

void CBox2DGDIDrawer::EndFrame()
{
	RECT windowRect;
	GetClientRect(window, &windowRect);
	BitBlt(frontBuffer, 0, 0, windowRect.right, windowRect.bottom, backBuffer, 0, 0, SRCCOPY);
	ValidateRect(window, &windowRect);
}

void CBox2DGDIDrawer::SetupFrame()
{
	RECT windowRect;
	GetClientRect(window, &windowRect);
	BitBlt(backBuffer, 0, 0, windowRect.right, windowRect.bottom, NULL, 0, 0, WHITENESS);
}
