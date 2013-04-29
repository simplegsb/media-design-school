#ifndef WINAPIENGINE_H_
#define WINAPIENGINE_H_

#include <string>
#include <vector>

#include <windows.h>

#include "Agent.h"
#include "InfluenceMap.h"
#include "Vector2.h"

class WinApiEngine
{
	public:
		WinApiEngine(const std::wstring& title, HINSTANCE instance, int commandShow);

		virtual ~WinApiEngine();
			
		void addRepulsionPoint(const Vector2& repulsionPoint);
			
		void addWaypoint(const Vector2& waypoint);
			
		void advance();
			
		void clearRepulsionPoints();
			
		void clearWaypoints();

		void destroy();

		const Agent* getAgent() const;

		int getHeight() const;

		const MSG& getMessage() const;

		const std::vector<Vector2>& getRepulsionPoints() const;

		const std::vector<Vector2>& getWaypoints() const;

		int getWidth() const;

		HWND getWindow() const;

		void init();

		void render();

		void renderAgent(HDC buffer);

		void renderInfluence(HDC buffer);

		void renderPoints(HDC buffer, const std::vector<Vector2>& points, bool joined);

		void setAgent(Agent* agent);

		void setDialog(HWND dialog);

		void setHeight(int height);

		void setWidth(int width);

	private:
		Agent* agent;

		HDC backBuffer;

		HBITMAP backBitmap;

		HBITMAP backBitmapOld;

		int commandShow;

		HWND dialog;

		HDC frontBuffer;

		int height;

		InfluenceMap influenceMap;

		HINSTANCE instance;

		MSG message;

		std::vector<Vector2> repulsionPoints;

		std::wstring title;

		std::vector<Vector2> waypoints;

		int width;

		HWND window;

		RECT getAdjustedRectangle() const;

		void registerWindowClass();
};

#endif /* WINAPIENGINE_H_ */
