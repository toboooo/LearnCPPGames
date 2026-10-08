#include <iostream>
#include "random.hpp"

char maze_lookup[32] = {
	1, 1, 1, 2,
	0, 0, 2, 2,
	1, 1, 1, 1,
	2, 0, 0, 0,
	1, 1, 1, 2,
	0, 0, 0, 0,
	2, 2, 1, 2,
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
			int a = i < 4 ? 0 : maze_line[i-2];
			int b = i < 3 ? 1 : maze_line[i-1];
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
