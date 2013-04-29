#include "stdafx.h"

#include "Ternary.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[]);
CTern equation1Left(CTern _a, CTern _b);
CTern equation1Right(CTern _a, CTern _b);
CTern equation2Left(CTern _a, CTern _b);
CTern equation2Right(CTern _a, CTern _b);
CTern equation3Left(CTern _a, CTern _b);
CTern equation3Right(CTern _a, CTern _b);
void printRow(CTern _a, CTern _b);
void printRow(CTern _a, CTern _b, CTern _c);
void printRow(CTern _a, CTern _b, CTern _left, CTern _right);

int _tmain(int argc, _TCHAR* argv[])
{
	cout << "| " << "NOT" << endl;
	cout << "| A\t\t | !A\t\t |" << endl;
	CTern a(CTern::TRUE);
	printRow(a, !a);
	a = CTern::UNKNOWN;
	printRow(a, !a);
	a = CTern::FALSE;
	printRow(a, !a);
	cout << endl;
	cout << "| " << "OR" << endl;
	cout << "| A\t\t | B\t\t | C\t\t |" << endl;
	a = CTern::TRUE;
	CTern b(CTern::TRUE);
	printRow(a, b, a || b);
	b = CTern::UNKNOWN;
	printRow(a, b, a || b);
	b = CTern::FALSE;
	printRow(a, b, a || b);
	a = CTern::UNKNOWN;
	b = CTern::TRUE;
	printRow(a, b, a || b);
	b = CTern::UNKNOWN;
	printRow(a, b, a || b);
	b = CTern::FALSE;
	printRow(a, b, a || b);
	a = CTern::FALSE;
	b = CTern::TRUE;
	printRow(a, b, a || b);
	b = CTern::UNKNOWN;
	printRow(a, b, a || b);
	b = CTern::FALSE;
	printRow(a, b, a || b);
	cout << endl;
	cout << "| " << "AND" << endl;
	cout << "| A\t\t | B\t\t | C\t\t |" << endl;
	a = CTern::TRUE;
	b = CTern::TRUE;
	printRow(a, b, a && b);
	b = CTern::UNKNOWN;
	printRow(a, b, a && b);
	b = CTern::FALSE;
	printRow(a, b, a && b);
	a = CTern::UNKNOWN;
	b = CTern::TRUE;
	printRow(a, b, a && b);
	b = CTern::UNKNOWN;
	printRow(a, b, a && b);
	b = CTern::FALSE;
	printRow(a, b, a && b);
	a = CTern::FALSE;
	b = CTern::TRUE;
	printRow(a, b, a && b);
	b = CTern::UNKNOWN;
	printRow(a, b, a && b);
	b = CTern::FALSE;
	printRow(a, b, a && b);
	cout << endl;
	cout << "| " << "IMPLIES" << endl;
	cout << "| A\t\t | B\t\t | C\t\t |" << endl;
	a = CTern::TRUE;
	b = CTern::TRUE;
	printRow(a, b, a.implies(b));
	b = CTern::UNKNOWN;
	printRow(a, b, a.implies(b));
	b = CTern::FALSE;
	printRow(a, b, a.implies(b));
	a = CTern::UNKNOWN;
	b = CTern::TRUE;
	printRow(a, b, a.implies(b));
	b = CTern::UNKNOWN;
	printRow(a, b, a.implies(b));
	b = CTern::FALSE;
	printRow(a, b, a.implies(b));
	a = CTern::FALSE;
	b = CTern::TRUE;
	printRow(a, b, a.implies(b));
	b = CTern::UNKNOWN;
	printRow(a, b, a.implies(b));
	b = CTern::FALSE;
	printRow(a, b, a.implies(b));
	cout << endl;
	cout << "| " << "EQUIVALENT" << endl;
	cout << "| A\t\t | B\t\t | C\t\t |" << endl;
	a = CTern::TRUE;
	b = CTern::TRUE;
	printRow(a, b, a == b);
	b = CTern::UNKNOWN;
	printRow(a, b, a == b);
	b = CTern::FALSE;
	printRow(a, b, a == b);
	a = CTern::UNKNOWN;
	b = CTern::TRUE;
	printRow(a, b, a == b);
	b = CTern::UNKNOWN;
	printRow(a, b, a == b);
	b = CTern::FALSE;
	printRow(a, b, a == b);
	a = CTern::FALSE;
	b = CTern::TRUE;
	printRow(a, b, a == b);
	b = CTern::UNKNOWN;
	printRow(a, b, a == b);
	b = CTern::FALSE;
	printRow(a, b, a == b);
	cout << endl;
	cout << "| " << "EQUATION1" << endl;
	cout << "| A\t\t | B\t\t | Left\t\t | Right\t |" << endl;
	a = CTern::TRUE;
	b = CTern::TRUE;
	printRow(a, b, equation1Left(a, b), equation1Right(a, b));
	b = CTern::UNKNOWN;
	printRow(a, b, equation1Left(a, b), equation1Right(a, b));
	b = CTern::FALSE;
	printRow(a, b, equation1Left(a, b), equation1Right(a, b));
	a = CTern::UNKNOWN;
	b = CTern::TRUE;
	printRow(a, b, equation1Left(a, b), equation1Right(a, b));
	b = CTern::UNKNOWN;
	printRow(a, b, equation1Left(a, b), equation1Right(a, b));
	b = CTern::FALSE;
	printRow(a, b, equation1Left(a, b), equation1Right(a, b));
	a = CTern::FALSE;
	b = CTern::TRUE;
	printRow(a, b, equation1Left(a, b), equation1Right(a, b));
	b = CTern::UNKNOWN;
	printRow(a, b, equation1Left(a, b), equation1Right(a, b));
	b = CTern::FALSE;
	printRow(a, b, equation1Left(a, b), equation1Right(a, b));
	cout << endl;
	cout << "| " << "EQUATION2" << endl;
	cout << "| A\t\t | B\t\t | Left\t\t | Right\t |" << endl;
	a = CTern::TRUE;
	b = CTern::TRUE;
	printRow(a, b, equation2Left(a, b), equation2Right(a, b));
	b = CTern::UNKNOWN;
	printRow(a, b, equation2Left(a, b), equation2Right(a, b));
	b = CTern::FALSE;
	printRow(a, b, equation2Left(a, b), equation2Right(a, b));
	a = CTern::UNKNOWN;
	b = CTern::TRUE;
	printRow(a, b, equation2Left(a, b), equation2Right(a, b));
	b = CTern::UNKNOWN;
	printRow(a, b, equation2Left(a, b), equation2Right(a, b));
	b = CTern::FALSE;
	printRow(a, b, equation2Left(a, b), equation2Right(a, b));
	a = CTern::FALSE;
	b = CTern::TRUE;
	printRow(a, b, equation2Left(a, b), equation2Right(a, b));
	b = CTern::UNKNOWN;
	printRow(a, b, equation2Left(a, b), equation2Right(a, b));
	b = CTern::FALSE;
	printRow(a, b, equation2Left(a, b), equation2Right(a, b));
	cout << endl;
	cout << "| " << "EQUATION3" << endl;
	cout << "| A\t\t | B\t\t | Left\t\t | Right\t |" << endl;
	a = CTern::TRUE;
	b = CTern::TRUE;
	printRow(a, b, equation3Left(a, b), equation3Right(a, b));
	b = CTern::UNKNOWN;
	printRow(a, b, equation3Left(a, b), equation3Right(a, b));
	b = CTern::FALSE;
	printRow(a, b, equation3Left(a, b), equation3Right(a, b));
	a = CTern::UNKNOWN;
	b = CTern::TRUE;
	printRow(a, b, equation3Left(a, b), equation3Right(a, b));
	b = CTern::UNKNOWN;
	printRow(a, b, equation3Left(a, b), equation3Right(a, b));
	b = CTern::FALSE;
	printRow(a, b, equation3Left(a, b), equation3Right(a, b));
	a = CTern::FALSE;
	b = CTern::TRUE;
	printRow(a, b, equation3Left(a, b), equation3Right(a, b));
	b = CTern::UNKNOWN;
	printRow(a, b, equation3Left(a, b), equation3Right(a, b));
	b = CTern::FALSE;
	printRow(a, b, equation3Left(a, b), equation3Right(a, b));

	char end;
	cin >> end;

	return 0;
}

CTern equation1Left(CTern _a, CTern _b)
{
	return _a.implies(_b) && _b.implies(_a);
}

CTern equation1Right(CTern _a, CTern _b)
{
	return _a == _b;
}

CTern equation2Left(CTern _a, CTern _b)
{
	return !(_a || _b);
}

CTern equation2Right(CTern _a, CTern _b)
{
	return !_a && !_b;
}

CTern equation3Left(CTern _a, CTern _b)
{
	return !(_a && _b);
}

CTern equation3Right(CTern _a, CTern _b)
{
	return !_a || !_b;
}

void printRow(CTern _a, CTern _b)
{
	cout << "| " << _a << "\t | " << _b << "\t |" << endl;
}

void printRow(CTern _a, CTern _b, CTern _c)
{
	cout << "| " << _a << "\t | " << _b << "\t | " << _c << "\t |" << endl;
}

void printRow(CTern _a, CTern _b, CTern _left, CTern _right)
{
	cout << "| " << _a << "\t | " << _b << "\t | " << _left << "\t | " << _right << "\t |" << endl;
}
