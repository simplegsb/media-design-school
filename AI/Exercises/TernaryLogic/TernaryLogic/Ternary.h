#pragma once

#include <iostream>

#include "String.h"

class CTern
{
	public:
		enum EValue
		{
			FALSE,
			TRUE,
			UNKNOWN
		};

		friend std::ostream& operator<<(std::ostream& _rOs, const CTern& _krValue);

		CTern(EValue _value);

		CTern implies(const CTern& _krRhs);

		CTern operator&&(const CTern& _krRhs);

		CTern operator==(const CTern& _krRhs);
		
		CTern operator!();

		CTern operator!=(const CTern& _krRhs);

		CTern operator||(const CTern& _krRhs);

	private:
		EValue m_value;
};

std::ostream& operator<<(std::ostream& _rOs, const CTern& _krValue);
