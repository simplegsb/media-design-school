#include "stdafx.h"

#include <algorithm>

#include "Knapsack.h"

Knapsack::Knapsack(int knapsack_size) :
	m_knapsack_size(knapsack_size)
{
}

void Knapsack::add_items(int value, int capacity)
{
	item newItem;
	newItem.value = value;
	newItem.capacity = capacity;

	if (capacity <= m_knapsack_size)
	{
		m_items.push_back(newItem);
	}
}

void Knapsack::solve()
{
	init_knapsack_table();

	for (unsigned int item_index = 1; item_index <= m_items.size(); item_index++)
	{
		for (int capacity_index = 0; capacity_index <= m_knapsack_size; capacity_index++)
		{
			item& current_item = m_items[item_index - 1];

			if (capacity_index >= current_item.capacity)
			{
				int used_capacity = m_knapsack_table[item_index - 1][capacity_index - current_item.capacity]
					+ current_item.value;

				if (used_capacity > m_knapsack_table[item_index - 1][capacity_index])
				{
					m_knapsack_table[item_index].push_back(used_capacity);
					m_selection_table[item_index].push_back(1);
					continue;
				}
			}

			m_knapsack_table[item_index].push_back(m_knapsack_table[item_index - 1][capacity_index]);
			m_selection_table[item_index].push_back(0);
		}
	}
}

void Knapsack::get_items_selected(std::vector<item>& resultItems) const
{
	int free_capacity = m_knapsack_size;
	for (int item_index = m_items.size(); item_index > 0; item_index--)
	{
		if (m_selection_table[item_index][free_capacity] == 1)
		{
			resultItems.push_back(m_items[item_index - 1]);
			free_capacity = free_capacity - m_items[item_index - 1].capacity;
		}
	}
}

void Knapsack::init_knapsack_table()
{
	m_knapsack_table.reserve(m_items.size());

	for (unsigned int index = 0; index <= m_items.size(); index++)
	{
		m_knapsack_table.push_back(std::vector<int>());
		m_knapsack_table[index].reserve(m_knapsack_size + 1);
	}

	for (int index = 0; index <= m_knapsack_size; index++)
	{
		m_knapsack_table[0].push_back(0);
	}

	m_selection_table.reserve(m_items.size());

	for (unsigned int index = 0; index <= m_items.size(); index++)
	{
		m_selection_table.push_back(std::vector<int>());
		m_selection_table[index].reserve(m_knapsack_size + 1);
	}
}

std::ostream& operator<<(std::ostream& stream, const Knapsack& knapsack)
{
	std::vector<item> selected_items;
	knapsack.get_items_selected(selected_items);

	for (unsigned int index = 0; index < selected_items.size(); index++)
	{
		stream << "[" << selected_items[index].capacity << "," << selected_items[index].value << "]" << std::endl;
	}

	return stream;
}
