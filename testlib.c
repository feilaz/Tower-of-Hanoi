#include "primlib.h"
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

#define POLES 3
#define DISCS 5

#define GAP_BETWEEN_DISCS 2

#define POLES_MAX_WIDTH 30

#define FLOOR_HIGHT 100

#define Y_VELOCITY 10
#define X_VELOCITY 10
#define X_DELAY 1
#define Y_DELAY 1

#define END_TEXT_LINE_1 "congratulations!!!"
#define END_TEXT_LINE_2 "You have WON!!!"

int disc_hight, disc_min_width, discs_width_difference, poles_width,
	poles_hight, max_hight_of_animated_disc;

int disc_placement[DISCS][2] = {0};
int coordinate_x[POLES];
int coordinate_y[DISCS];

void display_char(char key, int x, int y) {
	char print_string[2] = {key, '\0'};
	gfx_textout(x, y, print_string, WHITE);
}

void scale_size_of_structures() {
	disc_hight =
		(gfx_screenHeight() - FLOOR_HIGHT) / (2 * DISCS + GAP_BETWEEN_DISCS);
	discs_width_difference = gfx_screenWidth() / (POLES * 2 * DISCS);
	poles_width = POLES_MAX_WIDTH - 2 * POLES;
	disc_min_width = poles_width + 2 * discs_width_difference;
	poles_hight = (DISCS + 1) * (disc_hight + GAP_BETWEEN_DISCS);
	max_hight_of_animated_disc =
		gfx_screenHeight() - (poles_hight + FLOOR_HIGHT + 2 * disc_hight);
}

void draw_discs() {
	for (int n = 0; n < DISCS; ++n) {
		gfx_filledRect(disc_placement[n][0] -
						   (discs_width_difference * n + disc_min_width / 2),
					   disc_placement[n][1],
					   disc_placement[n][0] +
						   (discs_width_difference * n + disc_min_width / 2),
					   disc_placement[n][1] - disc_hight, GREEN);
	}
}

void draw_background() {
	gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BLACK);
	gfx_filledRect(0, gfx_screenHeight() - FLOOR_HIGHT, gfx_screenWidth() - 1,
				   gfx_screenHeight() - 1, BLUE);
	for (int n = 0; n < POLES; n++) {
		gfx_filledRect(coordinate_x[n] - poles_width / 2,
					   gfx_screenHeight() - FLOOR_HIGHT - 1,
					   coordinate_x[n] + poles_width / 2,
					   gfx_screenHeight() - FLOOR_HIGHT - poles_hight, YELLOW);
	}
}

int how_many_discs_on_the_pole(int pole_num) {
	int disc_count = 0;
	for (int n = 0; n < DISCS; n++) {
		if (disc_placement[n][0] == coordinate_x[pole_num]) {
			disc_count++;
		}
	}
	return disc_count;
}

void display_end_text() {
	gfx_textout(gfx_screenWidth() / 2 - 60, gfx_screenHeight() / 2 - 100,
				END_TEXT_LINE_1, WHITE);
	gfx_textout(gfx_screenWidth() / 2 - 50, gfx_screenHeight() / 2 - 50,
				END_TEXT_LINE_2, WHITE);
}

int check_winning_conditions() {
	if (how_many_discs_on_the_pole(POLES - 1) == DISCS) {
		return 1;
	} else {
		return 0;
	}
}

void draw_all_structures(int key1, int key2) {
	draw_background();
	draw_discs();
	display_char((char)(key1 + (int)'1'), gfx_screenWidth() - 50, 30);
	display_char((char)(key2 + (int)'1'), gfx_screenWidth() - 30, 30);
	gfx_updateScreen();
}

void draw_all_structures_and_end_text(int key1, int key2) {
	draw_all_structures(key1, key2);
	display_end_text();
	gfx_updateScreen();
}

void init_coordinates() {
	for (int n = 0; n < POLES; n++) {
		coordinate_x[n] = gfx_screenWidth() * ((n * 2) + 1) / (POLES * 2);
	}

	for (int n = 0; n < DISCS; ++n) {
		coordinate_y[n] = gfx_screenHeight() - (FLOOR_HIGHT + n * disc_hight +
												n * GAP_BETWEEN_DISCS + 1);
	}
}

void init_starting_discs_positions() {
	for (int n = 0; n < DISCS; n++) {
		disc_placement[n][0] = coordinate_x[0];
		disc_placement[DISCS - n - 1][1] = coordinate_y[n];
	}
}

int is_input_correct(int key) {
	for (int n = 1; n <= POLES; n++) {
		if ((int)key == ((int)'0' + n)) {
			return 1;
		}
	}
	if (POLES == 10 && key == '0') {
		return 1;
	}
	return 0;
}

int get_input() {
	char key;
	do {
		key = (char)gfx_getkey();
	} while (!is_input_correct(key));
	if (POLES == 10 && key == '0') {
		key += 10;
	}
	return (int)key - (int)'1';
}

int find_the_disc_on_top_of_the_pole(int pole_num) {
	int biggest_y = gfx_screenHeight(), disc_num = DISCS + 1;
	for (int n = 0; n < DISCS; n++) {
		if (disc_placement[n][0] == coordinate_x[pole_num] &&
			disc_placement[n][1] < biggest_y) {
			disc_num = n;
			biggest_y = disc_placement[n][1];
		}
	}
	return disc_num;
}

int is_move_possible(int key1, int key2) {
	int disc_to_be_moved = DISCS + 1, disc_to_be_placed_on = DISCS;
	disc_to_be_moved = find_the_disc_on_top_of_the_pole(key1);
	disc_to_be_placed_on = find_the_disc_on_top_of_the_pole(key2);
	if (disc_to_be_moved < disc_to_be_placed_on) {
		return 1;
	} else {
		return 0;
	}
}

void move_disc_coordinates(int from_pole, int to_pole) {
	int disc_on_top = find_the_disc_on_top_of_the_pole(from_pole);
	disc_placement[disc_on_top][0] = coordinate_x[to_pole];
	disc_placement[disc_on_top][1] =
		coordinate_y[how_many_discs_on_the_pole(to_pole) - 1];
}

void move_up(int disc_to_move, int from_pole, int to_pole) {
	for (int disc_y_coordinate = disc_placement[disc_to_move][1];
		 disc_y_coordinate > max_hight_of_animated_disc;
		 disc_y_coordinate -= Y_VELOCITY) {
		disc_placement[disc_to_move][1] = disc_y_coordinate;
		draw_all_structures(from_pole, to_pole);
		SDL_Delay(Y_DELAY);
	}
	disc_placement[disc_to_move][1] = max_hight_of_animated_disc;
	draw_all_structures(from_pole, to_pole);
}

void move_down(int disc_to_move, int from_pole, int to_pole,
			   int discs_on_destination_pole) {
	for (int disc_y_coordinate = disc_placement[disc_to_move][1];
		 disc_y_coordinate < coordinate_y[discs_on_destination_pole];
		 disc_y_coordinate += Y_VELOCITY) {
		disc_placement[disc_to_move][1] = disc_y_coordinate;
		draw_all_structures(from_pole, to_pole);
		SDL_Delay(Y_DELAY);
	}
	disc_placement[disc_to_move][1] = coordinate_y[discs_on_destination_pole];
	draw_all_structures(from_pole, to_pole);
}

void move_sideways(int disc_to_move, int from_pole, int to_pole) {
	if (disc_placement[disc_to_move][0] < coordinate_x[to_pole]) {
		for (int disc_x_coordinate = disc_placement[disc_to_move][0];
			 disc_x_coordinate <= coordinate_x[to_pole];
			 disc_x_coordinate += X_VELOCITY) {
			disc_placement[disc_to_move][0] = disc_x_coordinate;
			draw_all_structures(from_pole, to_pole);
			SDL_Delay(X_DELAY);
		}
	} else {
		for (int disc_x_coordinate = disc_placement[disc_to_move][0];
			 disc_x_coordinate >= coordinate_x[to_pole];
			 disc_x_coordinate -= X_VELOCITY) {
			disc_placement[disc_to_move][0] = disc_x_coordinate;
			draw_all_structures(from_pole, to_pole);
			SDL_Delay(X_DELAY);
		}
	}
	disc_placement[disc_to_move][0] = coordinate_x[to_pole];
	draw_all_structures(from_pole, to_pole);
}

void animated_movement_of_discs(int from_pole, int to_pole) {
	int disc_to_move = find_the_disc_on_top_of_the_pole(from_pole),
		discs_on_destination_pole = how_many_discs_on_the_pole(to_pole);

	move_up(disc_to_move, from_pole, to_pole);
	move_sideways(disc_to_move, from_pole, to_pole);
	move_down(disc_to_move, from_pole, to_pole, discs_on_destination_pole);
}

int main() {
	int key1 = (int)(' ' - '1'), key2 = (int)(' ' - '1');
	if (gfx_init()) {
		exit(3);
	}
	scale_size_of_structures();
	init_coordinates();
	init_starting_discs_positions();
	draw_all_structures(key1, key2);
	for (;;) {
		if (check_winning_conditions() == 1) {
			draw_all_structures_and_end_text(key1, key2);
			gfx_getkey();
			break;
		}
		key1 = get_input();
		draw_all_structures(key1, key2);
		key2 = get_input();
		draw_all_structures(key1, key2);
		if (is_move_possible(key1, key2)) {
			animated_movement_of_discs(key1, key2);
		}
		draw_all_structures(key1, key2);
	}
	return 1;
}
