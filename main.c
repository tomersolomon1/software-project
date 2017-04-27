/*
 * main.c
 */

#include "main_aux.h"
#include "sp_nim.h"
#include "SPBufferset.h"
#include <stdio.h>
# define MAX_HEAPS 32

int main() {
	int heaps[MAX_HEAPS] = { 0 };
	int move_vec[2] = { 0 };
	SP_BUFF_SET();
	int heaps_number = get_N();
	int heaps_sizes = get_heap_sizes(heaps, heaps_number);
	int turn_number = 1;
	int turn = 0; /* 0 is the computer's turn, 1 is the users turn */
	if ((heaps_number != 0) && (heaps_sizes != 0)  && (turn_number < 100)) {
		int game_on = 1;
		print_game_status(heaps, heaps_number, turn_number);
		while((game_on == 1) && (turn_number < 100)) {
			turn_number++;
			if (turn == 0) {
				computer_move(heaps, heaps_number, move_vec);
				print_computer_move(move_vec[0], move_vec[1]);
			} else {
				player_turn(heaps, heaps_number);
			}
			print_game_status(heaps, heaps_number, turn_number);
			print_heaps(heaps, heaps_number);
			game_on = is_game_over(heaps, heaps_number);
			turn = 1 - turn;
		}
		turn ? printf("Computer wins!\n") : printf("You win!\n");
	}
	return 0;
}
