/*
 * sp_nim.c
 *
 *  created by: Tomer and Sapir, 27/4/17
 */
#include "main_aux.h"
#include "sp_nim.h"
#include <stdio.h>

void winning_heap_strategy(int heaps[], int heaps_number,
		int nim_sum, int move_vec[]) {
	int i = 0;
	int found_winning_heap = 0; /* defualt value */
	int heap_nim_sum = 0;
	while ((i < heaps_number) && (found_winning_heap == 0)) {
			heap_nim_sum = nim_sum ^ heaps[i];
			// printf("heap index: %d, heap_nim_sum: %d\n", i, heap_nim_sum);
			if (heap_nim_sum < heaps[i]) {
				move_vec[0] = i; /* the changed heap */
				move_vec[1] = heaps[i] - heap_nim_sum; /* items taken */
				heaps[i] = heap_nim_sum; /* we took heaps[i] - heap_nim_sum, so now we have heap_nim_sum */
				found_winning_heap = 1;
			}
			i++;
		}
}

void all_losing_strategy(int heaps[], int heaps_number, int move_vec[]) {
	int i = 0;
	while (i < heaps_number) {
		if (heaps[i] > 0) {
			heaps[i] -= 1;
			move_vec[0] = i; /* the changed heap */
			move_vec[1] = 1; /* items taken */
			break;
		}
		i++;
	}
}

void computer_move(int heaps[], int heaps_number, int move_vec[]) {
	int nim_sum = 0;
	int i = 0;

	/* compute nim_sum */
	for (; i < heaps_number; i++) {
		nim_sum ^= heaps[i];
		// printf("i: %d, nim-sum: %d\n", i, nim_sum);
	}

	if (nim_sum != 0) {
		winning_heap_strategy(heaps, heaps_number, nim_sum, move_vec);
	} else { /* nim_sum == 0 */
		all_losing_strategy(heaps, heaps_number, move_vec);
	}
}

int is_game_over(int heaps[], int heaps_number) {
	for (int i = 0; i < heaps_number; i++) {
		if (heaps[i] != 0) {
			return 1; /* game is still on */
		}
	}
	return 0; /* the game is over */
}


