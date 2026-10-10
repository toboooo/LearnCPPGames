#include <iostream>
#include "random.hpp"

// The lookup table works by ensuring that three rules are maintained:
// 1. No 2x2 squares of the same bit value
// 2. No bit shall be trapped between three of the opposite value
// 3. Do not allow a vertical path to be blocked by placing a 1
// Make sure to consider the possible values of the next bit, Y
// A random bit can be placed when none of the rules apply

// | |c|d|e|
// |a|b|X|Y|

// 00000 -> 1 (2x2 rule)
// 00001 -> 1 (2x2 rule)
// 00010 -> 1 (3 trap rule)
// 00011 -> 2 (vertical path not broken by X=1)

// 00100 -> 0 (X=1, Y=0 would trap X between 3 zeros)
// 00101 -> 0 (3 trap rule)
// 00110 -> 2 (X=1 does not block a vertical path)
// 00111 -> 2 (X=1 does not block a vertical path)

// 01000 -> 1 (3 trap rule)
// 01001 -> 1 (3 trap rule)
// 01010 -> 1 (3 trap rule)
// 01011 -> 1 (3 trap rule)

// 01100 -> 2 (X=1 still allows a vertical path if Y=0)
// 01101 -> 0 (3 trap rule)
// 01110 -> 0 (2x2 rule)
// 01111 -> 0 (2x2 rule)

// 10000 -> 1 (2x2 rule)
// 10001 -> 1 (2x2 rule)
// 10010 -> 1 (3 trap rule)
// 10011 -> 2 (X=1 does not block a vertical path)

// 10100 -> 0 (3 trap rule)
// 10101 -> 0 (3 trap rule)
// 10110 -> 0 (3 trap rule)
// 10111 -> 0 (3 trap rule)

// 11000 -> 2 (X=1 does not necessarily block a vertical path (if Y=0))
// 11001 -> 0 (X=1 would break a vertical path)
// 11010 -> 1 (3 trap rule)
// 11011 -> 2 (No vertical path is available anyway)

// 11100 -> 2 (X=1 does not necessarily block a vertical path (if Y=0))
// 11101 -> 0 (3 trap rule)
// 11110 -> 0 (2x2 rule)
// 11111 -> 0 (2x2 rule)

char maze_lookup[32] = {
	1, 1, 1, 2,
	0, 0, 2, 2,
	1, 1, 1, 1,
	2, 0, 0, 0,
	1, 1, 1, 2,
	0, 0, 0, 0,
	2, 0, 1, 2,
	2, 0, 0, 0
};

void print_maze_line(char *maze_line, int len, char *output_buf) {
	for (int i = 0; i < len; ++i) {
		if (maze_line[i] == 0) {
			output_buf[i] = ' ';
		}
		else {
			output_buf[i] = '@';
		}
	}
	std::cout << output_buf;
}

void print_fully_connected_maze(int width, int n_lines) {
	int full_width = 2 * width + 4;
	char *output_buf = new char[full_width+2];
	output_buf[full_width] = '\n';
	output_buf[full_width+1] = '\0';
	char *prev_line = new char[full_width];
	char *maze_line = new char[full_width];
	prev_line[0] = 1;
	prev_line[1] = 1;
	maze_line[0] = 1;
	maze_line[1] = 1;
	for (int i = 0; i < width; ++i) {
		prev_line[i+2] = 0;
		prev_line[full_width-2-i-1] = 0;
	}
	prev_line[full_width-2] = 1;
	prev_line[full_width-1] = 1;
	maze_line[full_width-2] = 1;
	maze_line[full_width-1] = 1;
	for (; n_lines > 0; --n_lines) {
		for (int i = 2; i < width + 2; ++i) {
			int a = i < 4 ? 1 : maze_line[i-2];
			int b = i < 3 ? 1 : maze_line[i-1];
			int c = i < 3 ? Random::get(0, 1) : prev_line[i-1];
			int d = prev_line[i];
			int e = i == width + 1 ? 1 : prev_line[i+1];
			int index = (a << 4) | (b << 3) | (c << 2) | (d << 1) | e;
			int lookup_value = maze_lookup[index];
			maze_line[i] = lookup_value == 2 ? Random::get(0, 1) : lookup_value;
			maze_line[full_width-i-1] = maze_line[i];
		}
		print_maze_line(maze_line, full_width, output_buf);
		char *temp = maze_line;
		maze_line = prev_line;
		prev_line = temp;
	}			
	delete[] maze_line;
	delete[] prev_line;
	delete[] output_buf;
}

void print_atari_maze(int width, int n_lines) {
	int full_width = 2 * width + 4;
	char *output_buf = new char[full_width+2];
	output_buf[full_width] = '\n';
	output_buf[full_width+1] = '\0';
	char *prev_line = new char[full_width];
	char *maze_line = new char[full_width];
	prev_line[0] = 1;
	prev_line[1] = 1;
	maze_line[0] = 1;
	maze_line[1] = 1;
	for (int i = 0; i < width; ++i) {
		prev_line[i+2] = 0;
		prev_line[full_width-2-i-1] = 0;
	}
	prev_line[full_width-2] = 1;
	prev_line[full_width-1] = 1;
	maze_line[full_width-2] = 1;
	maze_line[full_width-1] = 1;
	for (; n_lines > 0; --n_lines) {
		for (int i = 2; i < width + 2; ++i) {
			int a = i < 4 ? 1 : maze_line[i-2];
			int b = i < 3 ? 0 : maze_line[i-1];
			int c = i < 3 ? Random::get(0, 1) : prev_line[i-1];
			int d = prev_line[i];
			int e = i == width + 1 ? Random::get(0, 1) : prev_line[i+1];
			int index = (a << 4) | (b << 3) | (c << 2) | (d << 1) | e;
			int lookup_value = maze_lookup[index];
			maze_line[i] = lookup_value == 2 ? Random::get(0, 1) : lookup_value;
			maze_line[full_width-i-1] = maze_line[i];
		}
		print_maze_line(maze_line, full_width, output_buf);
		char *temp = maze_line;
		maze_line = prev_line;
		prev_line = temp;
	}			
	delete[] maze_line;
	delete[] prev_line;
	delete[] output_buf;
}

int main() {
	print_atari_maze(8, 22);
	//print_fully_connected_maze(8, 22);
	return 0;
}
