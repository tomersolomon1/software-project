/*
 * main_aux.c
 *
 *  Created on: Apr 27, 2017
 *      Author: sapir
 */
#include <stdio.h>
#include "main_aux.h"
#include "sp_nim.h"

//get N=number of heaps, returns 0 if invalid
int get_N() {
	char input;
	int N = 0;
	printf("Enter the number of heaps:\n");
	fflush(stdout);
	while (((input = getchar()) != EOF) && (input != '\n')) {
		if('0' <= input && input <= '9'){
			N = N * 10;
			N += (input - '0');
		}
		else{
			printf("Error: the number of heaps must be between 1 and 32.\n");
			fflush(stdout);
			return 0;
		}
	}
	if (N < 1 || 32 < N){
		printf("Error: the number of heaps must be between 1 and 32.\n");
		fflush(stdout);
		return 0;
	}
	return N;
}

//get heaps size and start the heaps[]. returns 0 if not valid
//assuming the user gives N sizes
int get_heap_sizes(int heaps[]){
	char input;
	int i = 0;
	int curr_size = 0;
	printf("Enter the heap sizes:\n");
	fflush(stdout);
	while (((input = getchar()) != EOF) && (input != '\n')) {
		if(('0' <= input) && (input <= '9')){
			curr_size *= 10;
			curr_size += (input - '0');
		}
		else if(input == ' '){
			if(curr_size == 0){
				printf("Error: the size of heap %d should be positive.\n", i+1);
				fflush(stdout);
				return 0;
			}
			heaps[i] = curr_size;
			curr_size = 0;
			i++;
		}
		else {
			printf("Error: the size of heap %d should be positive.\n", i+1);
			fflush(stdout);
			return 0;
		}
	}
	if(curr_size == 0){
		printf("Error: the size of heap %d should be positive.\n", i+1);
		fflush(stdout);
		return 0;
	}
	heaps[i] = curr_size;
	return 1;
}

//prints heaps[] status as text
void print_game_status(int heaps[], int N, int turn_number){
	printf("In turn %d heap sizes are:", turn_number);
	fflush(stdout);
	for(int i=0; i < N; i++){
		printf(" h%d=%d", i+1, heaps[i]);
		fflush(stdout);
	}
	printf(".\n");
	fflush(stdout);
}

//print heaps visually
void print_heaps(int heaps[], int N){
	int max_objects = 0;
	int num_spaces[MAX_HEAPS] = {0};
	for(int i=0; i < N; i++){
		if (heaps[i] > max_objects){ max_objects = heaps[i];}
	}

	for(int i=0; i < N; i++){
		num_spaces[i] = max_objects - heaps[i];
	}
	for(int row=0; row < max_objects; row++){
		for(int col=0; col < N; col++){
			if(num_spaces[col] > 0){
				printf(" ");
				fflush(stdout);
				num_spaces[col] -= 1;
			}
			else{
				printf("*");
				fflush(stdout);
			}
			if(col < (N-1)) { printf("\t"); }
		}
		printf("\n");
		fflush(stdout);
	}
}

//player turn. plays until valid
void player_turn(int heaps[], int N){
	int is_valid_turn = 0;
	printf("Your turn: please enter the heap index and the number of removed objects.\n");
	fflush(stdout);
	while((is_valid_turn != 1)){
		is_valid_turn = player_turn_2(heaps, N);
	}
}

//player turn. returns 0 if not valid
int player_turn_2(int heaps[], int N){
	int heap_index = 0;
	int amount_removed = 0;
	char input;
	int q = 1; /*quest type: q=1 for heap_index, q=2 for amount_removed*/
	int had_error = 0;
	while (((input = getchar()) != EOF) && (input != '\n')) {
		if(had_error){ continue; }
		else if(q == 1){
			if('0' <= input && input <= '9'){
				heap_index = heap_index * 10;
				heap_index += (input - '0');
			}
			else if(input == ' '){
				q = 2;
				continue;
			}
			else{
				printf("Error: Invalid input.\nPlease enter again the heap index and the number of removed objects.\n");
				fflush(stdout);
				had_error = 1;
			}
		}
		else if(q == 2){
			if('0' <= input && input <= '9'){
				amount_removed = amount_removed * 10;
				amount_removed += (input - '0');
			}
			else{
				printf("Error: Invalid input.\nPlease enter again the heap index and the number of removed objects.\n");
				fflush(stdout);
				had_error = 1;
			}
		}
	}
	if(had_error){
		return 0;
	}
	else if(1 <= heap_index && heap_index <= N && heaps[heap_index - 1] >= amount_removed && amount_removed >= 1){
		printf("You take %d objects from heap %d.\n", amount_removed, heap_index);
		fflush(stdout);
		heaps[heap_index - 1] -= amount_removed;
		return 1;
	}
	else{
		printf("Error: Invalid input.\nPlease enter again the heap index and the number of removed objects.\n");
		fflush(stdout);
		return 0;
	}
}

//prints computer move
void print_computer_move(int heap_number, int amount_out){
	printf("Computer takes %d objects from heap %d.\n", amount_out, heap_number + 1);
	fflush(stdout);
}
