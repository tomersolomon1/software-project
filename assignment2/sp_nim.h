/*
 * sp_nim.h
 *
 *  Created on: 27 באפר 2017
 *      Author: User
 */

#ifndef SP_NIM_H_
#define SP_NIM_H_

void winning_heap_strategy(int heaps[], int heaps_number, int nim_sum, int move_vec[]);

void all_losing_strategy(int heaps[], int heaps_number, int move_vec[]);

void computer_move(int heaps[], int heaps_number, int move_vec[]);

int is_game_over(int heaps[], int heaps_number);

#endif /* SP_NIM_H_ */
