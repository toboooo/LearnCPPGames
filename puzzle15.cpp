#include <algorithm>
#include <array>
#include <iostream>
#include <limits>
#include "random.hpp"

constexpr int g_console_lines = 24;

class Direction {
	public:
	enum Type {
		up,
		down,
		left,
		right,
		max_directions,
	};

	explicit Direction(Type move) : m_dir(move) {}

	explicit Direction(char move) {
		switch (move) {
		case 'w':
			m_dir = up;
			break;
		case 's':
			m_dir = down;
			break;
		case 'a':
			m_dir = left;
			break;
		case 'd':
			m_dir = right;
			break;
		default:
			m_dir = max_directions;
		}
	}

	const Type dir() const {
		return m_dir;
	}

	private:
	Type m_dir;
};

class Board {
	public:
	Board();
	void display();
	void update(Direction move);
	bool check_win();

	private:
	bool check_valid_move(Direction move);
	void shuffle_board();

	std::array<int,16> m_board;
	int m_zero = 0;
};

Board::Board() {
	for (int i = 0; i < 15; ++i) {
		m_board[i] = i + 1;
	}
	m_board[15] = 0;
	m_zero = 15;
	shuffle_board();
}

void Board::display() {
	for (int i = 0; i < g_console_lines; ++i) {
		std::cout << '\n';
	}
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			int tile = m_board[i*4+j];
			int n_spaces = j == 0 ? 3 : 4;
			if (tile != 0) {
				n_spaces -= tile >= 10 ? 2 : 1;
			}
			for (int k = 0; k < n_spaces; ++k) {
				std::cout << ' ';
			}
			if (tile != 0) {
				std::cout << m_board[i*4+j];
			}
		}
		std::cout << '\n';
	}
}

void Board::update(Direction move) {
	if (!check_valid_move(move)) {
		return;
	}
	int target;
	switch (move.dir()) {
	case Direction::up:
		target = m_zero + 4;
		break;
	case Direction::down:
		target = m_zero - 4;
		break;
	case Direction::left:
		target = m_zero + 1;
		break;
	case Direction::right:
		target = m_zero - 1;
		break;
	}
	std::swap(m_board[target], m_board[m_zero]);
	m_zero = target;
}

bool Board::check_win() {
	for (int i = 0; i < 15; ++i) {
		if (m_board[i] != i + 1) {
			return false;
		}
	}
	if (m_board[15] != 0) {
		return false;
	}
	return true;
}

bool Board::check_valid_move(Direction move) {
	switch (move.dir()) {
	case Direction::up:
		return m_zero >= 12 ? false : true;
	case Direction::down:
		return m_zero <= 3 ? false : true;
	case Direction::left:
		return m_zero % 4 != 3;
	case Direction::right:
		return m_zero % 4 != 0;
	default:
		return false;
	}
}

void Board::shuffle_board() {
	Direction::Type moves[] = {Direction::up, Direction::down, Direction::left,
		Direction::right};
	for (int i = 0; i < 10000; ++i) {
		Direction::Type move = moves[Random::get(0,3)];
		update(Direction(move));
	}
}

namespace UserInput {
	bool get_input(char &input) {
		std::cin >> input;
		if (!std::cin || std::cin.peek() != '\n') {
			if (std::cin.eof()) {
				std::exit(0);
			}
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			return false;
		}
		if (input != 'w' && input != 'a' && input != 's' && input != 'd'
		&& input != 'q') {
			return false;
		}
		return true;
	}
};

int main() {
	Board board;
	while (true) {
		board.display();
		if (board.check_win()) {
			std::cout << "\nYOU WON!\n";
			break;
		}
		char move;
		bool check = UserInput::get_input(move);
		if (!check) {
			continue;
		}
		if (move == 'q') {
			std::cout << "Bye\n";
			break;
		}
		else {
			board.update(Direction(move));
		}
	}
	return 0;
}
