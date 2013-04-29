#include "stdafx.h"

#include "Ternary.h"

using namespace std;

CTern::CTern(EValue _value) :
	m_value(_value)
{
}

CTern CTern::implies(const CTern& _krRhs)
{
	if (m_value == _krRhs.m_value)
	{
		return CTern(TRUE);
	}

	if (m_value == FALSE)
	{
		return CTern(TRUE);
	}

	if (m_value == TRUE && _krRhs.m_value == UNKNOWN)
	{
		return CTern(UNKNOWN);
	}
	if (m_value == TRUE && _krRhs.m_value == FALSE)
	{
		return CTern(FALSE);
	}
	if (m_value == UNKNOWN && _krRhs.m_value == TRUE)
	{
		return CTern(TRUE);
	}
	if (m_value == UNKNOWN && _krRhs.m_value == FALSE)
	{
		return CTern(UNKNOWN);
	}
}

CTern CTern::operator==(const CTern& _krRhs)
{
	if (m_value == _krRhs.m_value)
	{
		return CTern(TRUE);
	}

	if (m_value == UNKNOWN || _krRhs.m_value == UNKNOWN)
	{
		return CTern(UNKNOWN);
	}

	return CTern(FALSE);
}

CTern CTern::operator&&(const CTern& _krRhs)
{
	if (m_value == TRUE && _krRhs.m_value == TRUE)
	{
		return CTern(TRUE);
	}

	if (m_value == FALSE || _krRhs.m_value == FALSE)
	{
		return CTern(FALSE);
	}

	return CTern(UNKNOWN);
}

CTern CTern::operator!()
{
	if (m_value == FALSE)
	{
		return CTern(TRUE);
	}
	
	if (m_value == TRUE)
	{
		return CTern(FALSE);
	}
	
	return CTern(UNKNOWN);
}

CTern CTern::operator!=(const CTern& _krRhs)
{
	return !(*this == _krRhs);
}

CTern CTern::operator||(const CTern& _krRhs)
{
	if (m_value == TRUE || _krRhs.m_value == TRUE)
	{
		return CTern(TRUE);
	}

	if (m_value == FALSE && _krRhs.m_value == FALSE)
	{
		return CTern(FALSE);
	}

	return CTern(UNKNOWN);
}

ostream& operator<<(ostream& _rOs, const CTern& _krValue)
{
	if (_krValue.m_value == CTern::TRUE)
	{
		return _rOs << "true   ";
	}

	if (_krValue.m_value == CTern::FALSE)
	{
		return _rOs << "false  ";
	}

	return _rOs << "unknown";
}
