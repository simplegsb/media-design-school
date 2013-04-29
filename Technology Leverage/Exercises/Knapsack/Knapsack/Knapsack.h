#include <ostream>
#include <vector>

struct item {
	int value;
	int capacity;
};

class Knapsack {
	public:
		Knapsack(int knapsack_size);

		void add_items(int value, int capacity);

		void solve();

		void get_items_selected(std::vector<item>& resultItems) const;

		friend std::ostream& operator<<(std::ostream& stream, const Knapsack& knapsack);

	private:
		void init_knapsack_table();

		std::vector<item> m_items;

		int m_knapsack_size;

		// 2D matrix to store intermediate
		// result of the knapsack calculation.
		std::vector<std::vector<int> > m_knapsack_table;

		// 2D matrix to store the intermediate
		// result for which item is selected
		// Later this matrix is used to get which items
		// are selected for optimal calculation.
		std::vector<std::vector<int> > m_selection_table;
};
