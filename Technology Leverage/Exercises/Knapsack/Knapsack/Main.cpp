#include "stdafx.h"

#include <iostream>
#include <string>

#include "Knapsack.h"

int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "What is the Knapsack capacity? ";
	std::string knapsack_capacity;
	std::cin >> knapsack_capacity;

	Knapsack knapsack(atoi(knapsack_capacity.c_str()));

	std::cout << "What do you want to do? (A)dd item, (S)olve: ";
	char option;
	std::cin >> option;

	std::string capacity;
	std::string value;

	while (option != 's' && option != 'S')
	{
		std::cout << "Enter item details:" << std::endl;
		std::cout << "\tCapacity: ";
		std::cin >> capacity;
		std::cout << "\tValue: ";
		std::cin >> value;

		knapsack.add_items(atoi(capacity.c_str()), atoi(value.c_str()));

		std::cout << "What do you want to do? (A)dd item, (S)olve: ";
		std::cin >> option;
	}

	knapsack.solve();

	return 0;
}
