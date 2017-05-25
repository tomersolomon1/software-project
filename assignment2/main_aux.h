/*
 * main_aux.h
 *
 *  Created on: 27 באפר 2017
 *      Author: User
 */

#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_
# define MAX_HEAPS 32

int player_turn_2(int heaps[], int N);

int get_N();

int get_heap_sizes(int heaps[]);

void print_game_status(int heaps[], int N, int turn_number);

void print_heaps(int heaps[], int N);

void player_turn(int heaps[], int N);

int player_turn_2(int heaps[], int N);

void print_computer_move(int heap_number, int amount_out);

#endif /* MAIN_AUX_H_ */
