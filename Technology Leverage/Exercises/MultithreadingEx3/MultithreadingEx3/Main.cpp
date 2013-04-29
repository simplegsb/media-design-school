
#include <process.h>
#include <sstream>
#include <string>
#include <windows.h>

#include "resource.h"

struct Equation
{
	TCHAR operand;
	double value1;
	double value2;
	double value3;
	double value4;
	double value5;
	double value6;
	double value7;
	double value8;
	double value9;
	double value10;
};

std::wstring calculate(const Equation& equation);
void calculateThread(void* equation);
void convertNumberSystem(char targetNumberSystem);
void convertNumberSystemThread(void* targetNumberSystem);
void convertScale(char targetFormat);
void convertScaleThread(void* targetFormat);
Equation* createEquation(HWND window, TCHAR operand);
BOOL CALLBACK DialogWndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
TCHAR getCharValue(HWND window, int controlId);
double getDoubleValue(HWND window, int controlId);
int WINAPI WinMain(HINSTANCE instance, HINSTANCE, PSTR, int);

bool calculatorClosed = false;
char numberSystem = 'd';
std::wstring result = L"0";
bool resultChanged = false;

std::wstring calculate(const Equation& equation)
{
	double resultDouble = 0.0;

	if (equation.operand == L'+')
	{
		resultDouble = equation.value1 + equation.value2;
	}
	else if (equation.operand == L'-')
	{
		resultDouble = equation.value1 - equation.value2;
	}
	else if (equation.operand == L'*')
	{
		resultDouble = equation.value1 * equation.value2;
	}
	else if (equation.operand == L'/')
	{
		resultDouble = equation.value1 / equation.value2;
	}
	else if (equation.operand == L'%')
	{
		resultDouble = equation.value1 / equation.value2 * 100.0;
	}
	else if (equation.operand == L'a')
	{
		double sum =
			equation.value1 +
			equation.value2 +
			equation.value3 +
			equation.value4 +
			equation.value5 +
			equation.value6 +
			equation.value7 +
			equation.value8 +
			equation.value9 +
			equation.value10;
		resultDouble = sum / 10.0;
	}

	std::wstringstream resultStream;
	resultStream << resultDouble;
	result = resultStream.str();

	return result;
}

void calculateThread(void* equation)
{
	result = calculate(*static_cast<Equation*>(equation));
	char currentNumberSystem = numberSystem;
	numberSystem = 'd';
	convertNumberSystem(currentNumberSystem);
	resultChanged = true;
	delete equation;
}

void convertNumberSystem(char targetNumberSystem)
{
	if (targetNumberSystem == numberSystem)
	{
		return;
	}

	if (targetNumberSystem == 'd')
	{
		if (numberSystem == 'b')
		{
			// HOW??!!
		}
	}
}

void convertNumberSystemThread(void* targetNumberSystem)
{
	convertNumberSystem(*static_cast<char*>(targetNumberSystem));
	delete targetNumberSystem;
}

void convertScale(char targetScale)
{
	double resultDouble = _wtof(result.c_str());

	if (targetScale == 'c')
	{
		resultDouble *= 30.48;
	}
	else if (targetScale == 'f')
	{
		resultDouble *= 0.0328084;
	}
	else if (targetScale == 'k')
	{
		resultDouble *= 0.453592;
	}
	else if (targetScale == 'p')
	{
		resultDouble *= 2.20462;
	}

	std::wstringstream resultStream;
	resultStream << resultDouble;
	result = resultStream.str();
}

void convertScaleThread(void* targetScale)
{
	convertScale(*static_cast<char*>(targetScale));
	resultChanged = true;
	delete targetScale;
}

Equation* createEquation(HWND window, TCHAR operand)
{
	Equation* equation = new Equation;
	equation->operand = operand;
	equation->value1 = getDoubleValue(window, IDC_VALUE1);
	equation->value2 = getDoubleValue(window, IDC_VALUE2);
	equation->value3 = getDoubleValue(window, IDC_VALUE3);
	equation->value4 = getDoubleValue(window, IDC_VALUE4);
	equation->value5 = getDoubleValue(window, IDC_VALUE5);
	equation->value6 = getDoubleValue(window, IDC_VALUE6);
	equation->value7 = getDoubleValue(window, IDC_VALUE7);
	equation->value8 = getDoubleValue(window, IDC_VALUE8);
	equation->value9 = getDoubleValue(window, IDC_VALUE9);
	equation->value10 = getDoubleValue(window, IDC_VALUE10);

	return equation;
}

BOOL CALLBACK DialogWndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_CLOSE)
	{
		DestroyWindow(window);
		calculatorClosed = true;
		return true;
	}
	else if (message == WM_COMMAND)
	{
		if (LOWORD(wParam) == IDC_ADD)
		{
			_beginthread(calculateThread, 0, static_cast<void*>(createEquation(window, '+')));
			return true;
		}
		else if (LOWORD(wParam) == IDC_AVERAGE)
		{
			_beginthread(calculateThread, 0, static_cast<void*>(createEquation(window, 'a')));
			return true;
		}
		else if (LOWORD(wParam) == IDC_BINARY)
		{
			char* numberSystem = new char;
			*numberSystem = 'b';
			_beginthread(convertNumberSystemThread, 0, numberSystem);
			return true;
		}
		else if (LOWORD(wParam) == IDC_CENT_TO_FEET)
		{
			char* conversion = new char;
			*conversion = 'f';
			_beginthread(convertScaleThread, 0, conversion);
			return true;
		}
		else if (LOWORD(wParam) == IDC_DECIMAL)
		{
			char* numberSystem = new char;
			*numberSystem = 'd';
			_beginthread(convertNumberSystemThread, 0, numberSystem);
			return true;
		}
		else if (LOWORD(wParam) == IDC_DIVIDE)
		{
			_beginthread(calculateThread, 0, static_cast<void*>(createEquation(window, '/')));
			return true;
		}
		else if (LOWORD(wParam) == IDC_FEET_TO_CENT)
		{
			char* conversion = new char;
			*conversion = 'c';
			_beginthread(convertScaleThread, 0, conversion);
			return true;
		}
		else if (LOWORD(wParam) == IDC_KILO_TO_POUND)
		{
			char* conversion = new char;
			*conversion = 'p';
			_beginthread(convertScaleThread, 0, conversion);
			return true;
		}
		else if (LOWORD(wParam) == IDC_MULTIPLY)
		{
			_beginthread(calculateThread, 0, static_cast<void*>(createEquation(window, '*')));
			return true;
		}
		else if (LOWORD(wParam) == IDC_PERCENTAGE)
		{
			_beginthread(calculateThread, 0, static_cast<void*>(createEquation(window, '%')));
			return true;
		}
		else if (LOWORD(wParam) == IDC_POUND_TO_KILO)
		{
			char* conversion = new char;
			*conversion = 'k';
			_beginthread(convertScaleThread, 0, conversion);
			return true;
		}
		else if (LOWORD(wParam) == IDC_SUBTRACT)
		{
			_beginthread(calculateThread, 0, static_cast<void*>(createEquation(window, '-')));
			return true;
		}
	}

	return false;
}

TCHAR getCharValue(HWND window, int controlId)
{
	TCHAR value[16];
	GetDlgItemText(window, controlId, value, 16);

	return *value;
}

double getDoubleValue(HWND window, int controlId)
{
	TCHAR value[16];
	GetDlgItemText(window, controlId, value, 16);

	return _wtof(value);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, PSTR, int)
{
	HWND calculator = CreateDialog(instance, MAKEINTRESOURCE(IDD_CALCULATOR), NULL, DialogWndProc);
	ShowWindow(calculator, SW_SHOW);

	SetDlgItemText(calculator, IDC_VALUE1, L"0");
	SetDlgItemText(calculator, IDC_VALUE2, L"0");
	SetDlgItemText(calculator, IDC_VALUE3, L"0");
	SetDlgItemText(calculator, IDC_VALUE4, L"0");
	SetDlgItemText(calculator, IDC_VALUE5, L"0");
	SetDlgItemText(calculator, IDC_VALUE6, L"0");
	SetDlgItemText(calculator, IDC_VALUE7, L"0");
	SetDlgItemText(calculator, IDC_VALUE8, L"0");
	SetDlgItemText(calculator, IDC_VALUE9, L"0");
	SetDlgItemText(calculator, IDC_VALUE10, L"0");

	MSG message;

	do
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			// It's poorly named, but IsDialogMessage actually forwards the message to the dialog.
			if(calculator == NULL || !IsDialogMessage(calculator, &message))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
		}

		if (resultChanged)
		{
			resultChanged = false;
			SetDlgItemText(calculator, IDC_RESULT, result.c_str());
		}
	}
	while (!calculatorClosed);

	return 0;
}
