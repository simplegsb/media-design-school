#include "WinApiEngine.h"

using namespace std;

WinApiEngine* staticInstance = NULL;

LRESULT CALLBACK handleEvent(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_DESTROY)
    {
        PostQuitMessage(0);
    }
	else if (message == WM_PAINT)
	{
		if (staticInstance != NULL)
		{
			staticInstance->render();
		}
	}
	else if (message == WM_LBUTTONUP)
	{
		int mouseX = static_cast<int>(LOWORD(lParam));
		int mouseY = static_cast<int>(HIWORD(lParam));
		//staticInstance->addWaypoint(Vector2((float) mouseX, (float) mouseY));
		staticInstance->addRepulsionPoint(Vector2((float) mouseX, (float) mouseY));
	}
	else if (message == WM_RBUTTONUP)
	{
		//staticInstance->clearWaypoints();
		staticInstance->clearRepulsionPoints();
	}
	else
	{
		return DefWindowProc(window, message, wParam, lParam);
	}

    return 0;
}

WinApiEngine::WinApiEngine(const wstring& title, HINSTANCE instance, int commandShow) :
		commandShow(commandShow), dialog(NULL), height(600), influenceMap(800 / 5, 600 / 5, 5), instance(instance),
		message(), title(title), waypoints(), width(800), window()
{
	staticInstance = this;
}

WinApiEngine::~WinApiEngine()
{
	delete agent;
}

void WinApiEngine::addRepulsionPoint(const Vector2& repulsionPoint)
{
	repulsionPoints.push_back(repulsionPoint);
	influenceMap.addRepulsionPoint(repulsionPoint);
}
		
void WinApiEngine::addWaypoint(const Vector2& waypoint)
{
	waypoints.push_back(waypoint);
}

void WinApiEngine::advance()
{
	if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
    {
		// It's poorly named, but IsDialogMessage actually forwards the message to the dialog.
		if(dialog == NULL || !IsDialogMessage(dialog, &message))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
    }
	
	SendMessage(window, WM_PAINT, NULL, NULL);
}

void WinApiEngine::clearRepulsionPoints()
{
	repulsionPoints.clear();
	influenceMap = InfluenceMap(800 / 5, 600 / 5, 5);
}

void WinApiEngine::clearWaypoints()
{
	waypoints.clear();
}

void WinApiEngine::destroy()
{
	SelectObject(backBuffer, backBitmapOld);
	DeleteObject(backBitmap);
	ReleaseDC(window, backBuffer);
	ReleaseDC(window, frontBuffer);
}

RECT WinApiEngine::getAdjustedRectangle() const
{
	RECT windowRectangle;
	windowRectangle.bottom = height;
	windowRectangle.left = 0;
	windowRectangle.right = width;
	windowRectangle.top = 0;

    AdjustWindowRect(&windowRectangle, WS_OVERLAPPEDWINDOW, false);
	
	return windowRectangle;
}

const Agent* WinApiEngine::getAgent() const
{
	return agent;
}

int WinApiEngine::getHeight() const
{
	return height;
}

const MSG& WinApiEngine::getMessage() const
{
	return message;
}

const vector<Vector2>& WinApiEngine::getRepulsionPoints() const
{
	return repulsionPoints;
}

const vector<Vector2>& WinApiEngine::getWaypoints() const
{
	return waypoints;
}

int WinApiEngine::getWidth() const
{
	return width;
}

HWND WinApiEngine::getWindow() const
{
	return window;
}

void WinApiEngine::init()
{
	registerWindowClass();

	RECT adjustedRectangle = getAdjustedRectangle();
	int adjustedWidth = adjustedRectangle.right - adjustedRectangle.left;
	int adjustedHeight = adjustedRectangle.bottom - adjustedRectangle.top;

	window = CreateWindowEx(NULL, L"WindowClass", title.data(), WS_OVERLAPPEDWINDOW, 0, 0, adjustedWidth,
		adjustedHeight, NULL, NULL, instance, NULL);

    ShowWindow(window, commandShow);

	RECT windowRect;
	GetClientRect(window, &windowRect);

	frontBuffer = GetDC(window);
	backBuffer = CreateCompatibleDC(frontBuffer);
	backBitmap = CreateCompatibleBitmap(frontBuffer, windowRect.right, windowRect.bottom);
	backBitmapOld = (HBITMAP) SelectObject(backBuffer, backBitmap);
}

void WinApiEngine::render()
{
	RECT windowRect;
	GetClientRect(window, &windowRect);

	BitBlt(backBuffer, 0, 0, windowRect.right, windowRect.bottom, NULL, 0, 0, WHITENESS);

	//renderInfluence(backBuffer);
	renderAgent(backBuffer);
	renderPoints(backBuffer, repulsionPoints, false);
	renderPoints(backBuffer, waypoints, true);

	BitBlt(frontBuffer, 0, 0, windowRect.right, windowRect.bottom, backBuffer, 0, 0, SRCCOPY);

	ValidateRect(window, &windowRect);
}

void WinApiEngine::renderAgent(HDC buffer)
{
	if (agent == NULL)
	{
		return;
	}

	HPEN aiPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	SelectObject(buffer, aiPen);

	float radius = agent->desiredPositionRadius;
	MoveToEx(buffer, (int) agent->position.x, (int) agent->position.y, NULL);
	LineTo(buffer, (int) agent->desiredPosition.x, (int) agent->desiredPosition.y);
	Ellipse(buffer, (int) agent->desiredPosition.x - radius, (int) agent->desiredPosition.y - radius, (int) agent->desiredPosition.x + radius, (int) agent->desiredPosition.y + radius);

	HPEN agentPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	DeleteObject(SelectObject(buffer, agentPen));

	Vector2 toProjectedPosition = agent->velocity;
	toProjectedPosition.normalise();
	toProjectedPosition *= 10.0f;
	Vector2 projectedPosition = agent->position + toProjectedPosition;

	MoveToEx(buffer, (int) agent->position.x, (int) agent->position.y, NULL);
	LineTo(buffer, (int) projectedPosition.x, (int) projectedPosition.y);
	Ellipse(buffer, (int) agent->position.x - 5, (int) agent->position.y - 5, (int) agent->position.x + 5, (int) agent->position.y + 5);

	//POINT poly[] = {p0x, p0y, p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y, p5x, p5y, p6x, p6y};
	//Polygon(buffer, poly, sizeof(poly)/(sizeof(poly[0])*2));*/

	DeleteObject(agentPen);
}

void WinApiEngine::renderInfluence(HDC buffer)
{
	for (unsigned int gridX = 0; gridX < 800 / 5; gridX++)
	{
		for (unsigned int gridY = 0; gridY < 600 / 5; gridY++)
		{
			int influenceScalar = influenceMap.getInfluenceScalar(gridX, gridY);
			if (influenceScalar > 0)
			{
				HPEN influencePen = CreatePen(PS_SOLID, 1, RGB(255 - (5 * influenceScalar), 255 - (5 * influenceScalar), 255 - (5 * influenceScalar)));
				SelectObject(buffer, influencePen);
				HBRUSH influenceBrush = CreateSolidBrush(RGB(255 - (5 * influenceScalar), 255 - (5 * influenceScalar), 255 - (5 * influenceScalar)));
				SelectObject(buffer, influenceBrush);

				Rectangle(buffer, gridX * 5, gridY * 5, (gridX + 1) * 5, (gridY + 1) * 5);

				DeleteObject(influencePen);
				DeleteObject(influenceBrush);
			}
		}
	}
}

void WinApiEngine::renderPoints(HDC buffer, const vector<Vector2>& points, bool joined)
{
	HPEN redPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	SelectObject(buffer, redPen);

	for (unsigned int index = 0; index < points.size(); index++)
	{
		Ellipse(buffer, (int) points.at(index).x - 5, (int) points.at(index).y - 5, (int) points.at(index).x + 5, (int) points.at(index).y + 5);
		
		if (joined)
		{
			if (index == 0)
			{
				MoveToEx(buffer, (int) points.at(index).x, (int) points.at(index).y, NULL);
			}
			else
			{
				LineTo(buffer, (int) points.at(index).x, (int) points.at(index).y);
			}
		}
	}

	DeleteObject(redPen);
}

void WinApiEngine::registerWindowClass()
{
	WNDCLASSEX windowClass;
    ZeroMemory(&windowClass, sizeof(WNDCLASSEX));

    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hInstance = instance;
    windowClass.lpfnWndProc = handleEvent;
    windowClass.lpszClassName = L"WindowClass";
    windowClass.style = CS_HREDRAW | CS_VREDRAW;

    RegisterClassEx(&windowClass);
}

void WinApiEngine::setAgent(Agent* agent)
{
	this->agent = agent;
}

void WinApiEngine::setDialog(HWND dialog)
{
	this->dialog = dialog;
}

void WinApiEngine::setHeight(int height)
{
	this->height = height;
}

void WinApiEngine::setWidth(int width)
{
	this->width = width;
}
