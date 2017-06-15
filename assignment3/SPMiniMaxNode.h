#ifndef SPMINIMAXNODE_H_
#define SPMINIMAXNODE_H_

#include "SPFIARGame.h"
#include <limits.h>
#include <stddef.h>


//Put all decleartions and constants here

/* struct move_value_t - the information relevant to specific move: the move itself, and it's value according to the mini-max algo */
typedef struct move_value_t {
	int move; /* the column */
	int value; /* the value of the move, according to the mini-max algo */
} move_value;

/* @param u - vector
 * @param u - vector
 * u and v are arrays of int
 *
 * @return
 * the inner product between v, u -> <v, u>
 */

int inner_product(int v[], int u[]);

/* @param winner - the number of the winner player (i.e. 1, 2)
 * @return
 * INT_MAX iff winner == 1 (otherwise return INT_MIN)
 */
int winning_value(int winner);

/* @param symbol - either the symbol of player 1 or 2, or the empty char
 * @param player1_seq - pointer to the counter of the player 1 symbols in the sequence
 * @param player2_seq - pointer to the counter of the player 1 symbols in the sequence
 *
 * if the symbol belongs to player 1, his counter is incremented
 * otherwise, checks if it belongs to player 2, and increment his counter if necessary
 */

void update_sequences_length(char symbol, int *player1_seq, int *player2_seq);

/* @param player1_counter - counter of the symbols of player 1 in the sequence
 * @param player2_counter - counter of the symbols of player 2 in the sequence
 * @param sequences_counter - store the diff in all sequences, keeps track of the sum-up of all the spans
 *
 * checks if either one of the players has a sequence of 4 - and therefore has won the game
 * otherwise, update sequences_counter according to the board-evaluation scheme
 *
 * @return
 * 1 	if player 1 won
 * 2 	if player 2 won
 * 0 	if nobody won (in this specific span)
 */

int span_sumup(int player1_counter, int player2_counter, int sequences_counter[]);

/* @param currentGame - pointer to the game
 * @param ri - starting row coordinate
 * @param cj - starting column coordinate
 * @param sequences_counter - store the diff in all sequences, keeps track of the sum-up of all the spans
 *
 * compute sequences in row - from (ri, cj) the right
 * updates sequences_counter
 *
 * @return
 * 1 	if player 1 won
 * 2 	if player 2 won
 * 0 	if nobody won (in this specific span)
 */

int span_row_right(SPFiarGame* currentGame, int ri, int cj, int sequences_counter[]);

/*
 * @param currentGame - pointer to the game
 * @param ri - starting row coordinate
 * @param cj - starting column coordinate
 * @param sequences_counter - store the diff in all sequences, keeps track of the sum-up of all the spans
 *
 * compute sequences in colum - from (ri, cj) upward
 * updates sequences_counter
 *
 * @return
 * 1 	if player 1 won
 * 2 	if player 2 won
 * 0 	if nobody won (in this specific span)
 */

int span_column_up(SPFiarGame* currentGame, int ri, int cj, int sequences_counter[]);

/*
 * @param currentGame - pointer to the game
 * @param ri - starting row coordinate
 * @param cj - starting column coordinate
 * @param sequences_counter - store the diff in all sequences, keeps track of the sum-up of all the spans
 *
 * compute sequences in diagonal - from (ri, cj) -  up-right
 * updates sequences_counter
 *
 * @return
 * 1 	if player 1 won
 * 2 	if player 2 won
 * 0 	if nobody won (in this specific span)
 */

int span_diagonal_upright(SPFiarGame* currentGame, int ri, int cj, int sequences_counter[]);

/* @param currentGame - pointer to the game
 * @param ri - starting row coordinate
 * @param cj - starting column coordinate
 * @param sequences_counter - store the diff in all sequences, keeps track of the sum-up of all the spans
 *
 * compute sequences in diagonal - from (ri, cj) -  up-left
 * updates sequences_counter
 *
 * @return
 * 1 	if player 1 won
 * 2 	if player 2 won
 * 0 	if nobody won (in this specific span)
 */

int span_diagonal_upleft(SPFiarGame* currentGame, int ri, int cj, int sequences_counter[]);

/* @param currentGame - a pointer to the game
 * gets a pointer to the game, and evaluates it's value according to the guidelines
 * return INT_MAX if player 1 won, INT_MIN if player 2 won, otherwise calculate the value of the board according to the spans scheme
 * the algorithm calculates sequences of player 1 & 2, and then sum it up with the weights vector [-5, -2, -1, 0, 1, 2, 5]
 * operates in O(n) complexity
 *
 * @return
 * INT_MAX 					if player 1 won
 * INT_MIN 					if player 2 won
 * the value of the board	if nobody won (in this specific span)
 */

int evaluate_board(SPFiarGame* currentGame);

/* @param best_move_so_far - a pointer to best move so far according to the mini-max algo
 * @param vlaue - the value of the new optional move
 * @param move  - the new optional move
 * @param current_player - the current player
 *
 * the object of player 1 is to maximize the score function, and the object of player 2 is to minimize it
 * therefore, the function updates the best move in these cases:
 * 		a. it's player 1 turn, and the value is higher than the value of the best-move-so-far
 * 		b. it's player 2 turn, and the value is lower  than the value of the best-move-so-far
 *		c. if best_move_so_far->move == -1, since it's the first move to be considered
 */

void update_best_move(move_value* best_move_so_far, int value, int move, char current_player);

/* @param game_copy - a pointer to the game
 * @param this_move - a pointer to the last move
 *
 * return 1 if the game is over, otherwise return 0
 * also, setting this_move.value = INT_MAX (if player 1 has won), this_move.value = INT_MIN (if player 2 has won)
 * and this_move.value = 0 (if it's a tie)
 *
 * @return
 * 1		if the game is over
 * 0		the game is still on
 */

int is_the_game_over(SPFiarGame* game_copy, move_value* this_move);

/* @param game_copy - a pointer to the game
 * @param maxDepth  - the maximum depth of the recursion
 *
 * finds the best move for the current player, according to the Mini-Max algo
 * the object of player 1 is to maximize the value-function, and the object of player 2 is to minimize the value-function
 * return best_move (which is of type move_value):
 * 		a. if the game is not over yet, best_move is best move of the current player (according to the algo)
 * 		b. if the game is over (last move won it or made it a tie), best_move.move = -2, and:
 * 				1. it't a tie: best_move.value = 0
 * 				2. player 1 won: best_move.value = INT_MAX
 * 				3. player 2 won: best_move.value = INT_MIN
 *
 * assuming maxDepth >= 1
 * by default, |this_move.value| < INT_MAX, since value = INT_MAX means winning of player 1 (similarly, INT_MIN means the winning of player 2)
 *
 * @return best_move - the best move of the current player, according to the mini-max algo
 *
 */

move_value minimaxAlgo(SPFiarGame* game_copy, unsigned int maxDepth);

#endif
