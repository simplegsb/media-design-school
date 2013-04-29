#ifndef BOARD_H_
#define BOARD_H_

#include <string>
#include <vector>

namespace tictactoe
{
	class Board
	{
		friend bool operator==(const Board& lhs, const Board& rhs);

		public:
			Board();

			void clear(unsigned int x, unsigned int y);

			const std::string& get(unsigned int x, unsigned int y) const;

			bool isFull() const;

			bool lineOfThreeExists(const std::string symbol) const;

			void print() const;

			bool set(unsigned int x, unsigned int y, const std::string& symbol);

		private:
			std::vector<std::vector<std::string> > cells;

			bool diagonalLineOfThreeExists(const std::string symbol) const;

			bool horizontalLineOfThreeExists(const std::string symbol) const;

			bool verticalLineOfThreeExists(const std::string symbol) const;
	};

	bool operator!=(const Board& lhs, const Board& rhs);

	bool operator==(const Board& lhs, const Board& rhs);
}

#endif /* BOARD_H_ */
