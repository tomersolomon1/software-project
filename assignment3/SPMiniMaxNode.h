#ifndef SPMINIMAXNODE_H_
#define SPMINIMAXNODE_H_

#include "SPFIARGame.h"
#include <limits.h>
#include <stddef.h>


//Put all decleartions and constants here

typedef struct move_value_t {
	int move; /* the column */
	int value; /* the value of the move, according to the mini-max algo */
} move_value;


int inner_product(int v[], int u[]);

int winning_value(int winner);

/*assuming symbol is either the symbol of player 1 ot player 2 */
void update_sequences_length(char symbol, int *player1_seq, int *player2_seq);

int span_sumup(int player1_counter, int player2_counter, int sequences_counter[]);

/* compute sequences in row - to the right
 * returning 0 means no winner, 1 means player 1 won, 2 means player 2 won
 */
int span_row_right(SPFiarGame* currentGame, int ri, int cj, int sequences_counter[]);

/* compute sequences in column - up-ward
 * returning 0 means no winner, 1 means player 1 won, 2 means player 2 won
 */
int span_column_up(SPFiarGame* currentGame, int ri, int cj, int sequences_counter[]);

/* compute sequences in diagonal - up-right-ward
 * returning 0 means no winner, 1 means player 1 won, 2 means player 2 won
 */
int span_diagonal_upright(SPFiarGame* currentGame, int ri, int cj, int sequences_counter[]);

/* compute sequences in diagonal - up-left
 * returning 0 means no winner, 1 means player 1 won, 2 means player 2 won
 */
int span_diagonal_upleft(SPFiarGame* currentGame, int ri, int cj, int sequences_counter[]);

/*
 * the algorithm: calculating sequences of player 1 & 2, and then sum it up with the weights vector [-5, -2, -1, 0, 1, 2, 5]
 * operates in O(n) complexity
 */
int evaluate_board(SPFiarGame* currentGame);

/* we can assume the game is not over - there is at least one legal move, and the nobody has won yet */
int spMinimaxSuggestMove(SPFiarGame* currentGame, unsigned int maxDepth);

void update_best_move(move_value* best_move_so_far, int value, int move, char current_player);

/* checks if the game is over, and updates this
 * return 1 if the game is over
 * otherwise, return 0
 * also, setting this_move.value = INT_MAX (if player 1 has won), this_move.value = INT_MIN (if player 2 has won)
 * and this_move.value = 0 (if it's a tie)
 */
int is_the_game_over(SPFiarGame* game_copy, move_value* this_move);

char get_symbol(char current_player);

/* assuming maxDepth >= 1
 * the object of player 1 is to maximize the value-function, and the object of player 2 is to minimize the value-function
 * by default, |this_move.value| < INT_MAX, since value = INT_MAX is means the winning of player 1 (similarly, INT_MIN means the winning of player 2) */
move_value minimaxAlgo(SPFiarGame* game_copy, unsigned int maxDepth);

void check_eval1();

void check_eval2();

void check_eval3();

void check_eval();

#endif
