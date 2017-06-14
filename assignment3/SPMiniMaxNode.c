/*
 * SPMiniMaxNode.c
 *
 */

#include "SPFIARGame.h"
#include <limits.h>
#include <stddef.h>
#include "SPMiniMaxNode.h"

move_value minimaxAlgo(SPFiarGame* game_copy, unsigned int maxDepth);

/* gets tow vectors u, v (as arrays of int)
 * return the inner product between v, u -> <v, u>
 */
int inner_product(int v[], int u[]) {
	int product = 0;
	for (int i = 0; i < 7; i++) {
		product += v[i] * u[i];
	}
	return product;
}

/* gets the number of the winner player (i.e. 1, 2)
 * return INT_MAX iff winner == 1 (otherwise return INT_MIN)
 */

int winning_value(int winner) {
	int value;
	value = (winner == 1 ? INT_MAX : INT_MIN);
	return value;
}

/* gets a symbol, and pointers to counters, which count the number of symbols of player1 & player 2 (respectively)
 * if the symbol belongs to player 1, his counter is incremented
 * otherwise, checks if it belongs to player 2, and increment his counter if necessary
 */
void update_sequences_length(char symbol, int *player1_seq, int *player2_seq) {
	if (symbol == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
		*player1_seq += 1;
	} else if (symbol == SP_FIAR_GAME_PLAYER_2_SYMBOL) {
		 *player2_seq += 1;
	}
}

/* gets counters of player 1 and player 2, and the sequences_counter (sequences_counter keeps track of the sum-up of all the spans)
 * checks if either one of the players has a sequence of 4 - and therefore has won the game
 * otherwise, update sequences_counter according to the board-evaluation scheme
 * return 1 if player 1 won, 2 if player 2 won, 0 if nobody won (in this specific span
 */

int span_sumup(int player1_counter, int player2_counter, int sequences_counter[]) {
	int somebody_won = 0; /* 0 means no winner, 1 means player 1 won, 2 means player 2 won */
	if (player1_counter == 4 || player2_counter == 4) {
		somebody_won = (player1_counter == 4 ? 1 : 2);
	} else {
		int sequence_index = player1_counter - player2_counter + 3;
		sequences_counter[sequence_index] += 1;
	}
	return somebody_won;
}

/* gets a pointer to the game, coordinates (ri, cj) (the starting point of the span) and the sequences_counter (which keeps track of all the sequences)
 * compute sequences in row - from (ri, cj) the right
 * updates sequences_counter
 * return 0 if no winner, 1 if player 1 won, 2 if player 2 won
 */

int span_row_right(SPFiarGame* currentGame, int ri, int cj, int sequences_counter[]) {
	int player1_counter = 0;
	int player2_counter = 0;
	if (cj+3 < SP_FIAR_GAME_N_COLUMNS) {
		for (int k = 0; k < 4; k++) {
			update_sequences_length(currentGame->gameBoard[ri][cj+k], &player1_counter, &player2_counter);
		}
	}
	return span_sumup(player1_counter, player2_counter, sequences_counter);
}

/* gets a pointer to the game, coordinates (ri, cj) (the starting point of the span) and the sequences_counter (which keeps track of all the sequences)
 * compute sequences in column - from (ri, cj) - up-ward
 * updates sequences_counter
 * return 0 if no winner, 1 if player 1 won, 2 if player 2 won
 */

int span_column_up(SPFiarGame* currentGame, int ri, int cj, int sequences_counter[]) {
	int player1_counter = 0;
	int player2_counter = 0;
	if (ri+3 < SP_FIAR_GAME_N_ROWS) {
		for (int k = 0; k < 4; k++) {
			update_sequences_length(currentGame->gameBoard[ri+k][cj], &player1_counter, &player2_counter);
		}
	}
	return span_sumup(player1_counter, player2_counter, sequences_counter);
}

/* gets a pointer to the game, coordinates (ri, cj) (the starting point of the span) and the sequences_counter (which keeps track of all the sequences)
 * compute sequences in diagonal - from (ri, cj) -  up-right
 * updates sequences_counter
 * return 0 if no winner, 1 if player 1 won, 2 if player 2 won
 */

int span_diagonal_upright(SPFiarGame* currentGame, int ri, int cj, int sequences_counter[]) {
	int player1_counter = 0;
	int player2_counter = 0;
	if ((ri+3 < SP_FIAR_GAME_N_ROWS) && (cj+3 < SP_FIAR_GAME_N_COLUMNS)) {
		for (int k = 0; k < 4; k++) {
			update_sequences_length(currentGame->gameBoard[ri+k][cj+k], &player1_counter, &player2_counter);
		}
	}
	return span_sumup(player1_counter, player2_counter, sequences_counter);
}

/* gets a pointer to the game, coordinates (ri, cj) (the starting point of the span) and the sequences_counter (which keeps track of all the sequences)
 * compute sequences in diagonal - from (ri, cj) -  up-left
 * updates sequences_counter
 * return 0 if no winner, 1 if player 1 won, 2 if player 2 won
 */

int span_diagonal_upleft(SPFiarGame* currentGame, int ri, int cj, int sequences_counter[]) {
	int player1_counter = 0;
	int player2_counter = 0;
	if ((ri+3 < SP_FIAR_GAME_N_ROWS) && (cj-3 >= 0)) {
		for (int k = 0; k < 4; k++) {
			update_sequences_length(currentGame->gameBoard[ri+k][cj-k], &player1_counter, &player2_counter);
		}
	}
	return span_sumup(player1_counter, player2_counter, sequences_counter);
}

/* gets a pointer to the game, and evaluates it's value according to the guidelines
 * return INT_MAX if player 1 won, INT_MIN if player 2 won, otherwise calculate the value of the board according to the spans scheme
 * the algorithm calculates sequences of player 1 & 2, and then sum it up with the weights vector [-5, -2, -1, 0, 1, 2, 5]
 * operates in O(n) complexity
 */
int evaluate_board(SPFiarGame* currentGame) {
	int sequences_counter[7] = { 0 };
	int weights[7] = {-5, -2, -1, 0, 1, 2, 5};
	int somebody_won = 0; /* somebody_won = 1 means player 1 won, somebody_won = 2 means player 2 won */
	for (int ri = 0; ri < SP_FIAR_GAME_N_ROWS; ri++) {
		for (int cj = 0; cj < SP_FIAR_GAME_N_COLUMNS; cj++) {
			somebody_won = span_diagonal_upleft(currentGame, ri, cj, sequences_counter);
			if (somebody_won != 0) { /* somebody won! */
				return winning_value(somebody_won);
			}
			somebody_won = span_diagonal_upright(currentGame, ri, cj, sequences_counter);
			if (somebody_won != 0) { /* somebody won! */
				return winning_value(somebody_won);
			}
			somebody_won = span_row_right(currentGame, ri, cj, sequences_counter);
			if (somebody_won != 0) { /* somebody won! */
				return winning_value(somebody_won);
			}
			somebody_won = span_column_up(currentGame, ri, cj, sequences_counter);
			if (somebody_won != 0) { /* somebody won! */
				return winning_value(somebody_won);
			}
		}
	}
	return inner_product(sequences_counter, weights); /* if we got here it means no one has won so far */
}

/* gets a pointer to best_move_so_far, the value of the new move, and the current player
 * the object of player 1 is to maximize the score function, and the object of player 2 is to minimize it
 * therefore, the function updates the best move in these cases:
 * 		a. it's player 1 turn, and the value is higher than the value of the best-move-so-far
 * 		b. it's player 2 turn, and the value is lower  than the value of the best-move-so-far
 *		c. if best_move_so_far->move == -1, since it's the first move to be considered
 */
void update_best_move(move_value* best_move_so_far, int value, int move, char current_player) {
	if ((current_player == '1' && value > best_move_so_far->value) || (current_player == '2' && value < best_move_so_far->value)
			|| (best_move_so_far->move == -1)) {
		best_move_so_far->value = value;
		best_move_so_far->move = move;
	}
}

/* gets a pointer to the game, and a pointer to this-move
 * return 1 if the game is over, otherwise return 0
 * also, setting this_move.value = INT_MAX (if player 1 has won), this_move.value = INT_MIN (if player 2 has won)
 * and this_move.value = 0 (if it's a tie)
 */
int is_the_game_over(SPFiarGame* game_copy, move_value* this_move) {
	char game_status = spFiarCheckWinner(game_copy);
	if (game_status == SP_FIAR_GAME_PLAYER_1_SYMBOL) { /* player 1 has won */
		this_move->value = INT_MAX;
		return 1;
	} else if (game_status == SP_FIAR_GAME_PLAYER_2_SYMBOL) { /* player 2 has won */
		this_move->value = INT_MIN;
		return 1;
	} else if (game_status == SP_FIAR_GAME_TIE_SYMBOL) { /* it's a tie */
		this_move->value = 0;
		return 1;
	}
	return 0; /* otherwise, no one has won and it's not a tie */
}

/*
 * gets the current_player ('1' or '2'), and returns it's symbol
 * to be deleted??????????????
 */

char get_symbol(char current_player) {
	char symbol = (current_player == '1' ? SP_FIAR_GAME_PLAYER_1_SYMBOL : SP_FIAR_GAME_PLAYER_2_SYMBOL);
	return symbol;
}

/* gets a pointer to the game, and the maxDepth of the recursion
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
 */
move_value minimaxAlgo(SPFiarGame* game_copy, unsigned int maxDepth) {
	char current_symbol =  get_symbol(game_copy->currentPlayer); /* the symbol of the current player */
	move_value best_move; /* will contain the best move for the player and it's value */
	best_move.move = -1; /* default value */
	best_move.value = (game_copy->currentPlayer == '1' ? (INT_MIN + 2) : (INT_MAX - 2)); /* default value for the mini-max algo */
	if (is_the_game_over(game_copy, &best_move)) { /* checking if the game is over, and if so - this_move.move will be -1 */
		best_move.move = -2; /* means the game is over */
		return best_move;
	}
	else if (maxDepth == 1) { /* halting condition of the recursion */
		for (int ci = 0; ci < SP_FIAR_GAME_N_COLUMNS; ci++) {
			int top_row_ci = game_copy->tops[ci]; /* the first available row in this column  */
			if (top_row_ci < SP_FIAR_GAME_N_ROWS) { /* column isn't full */
				game_copy->gameBoard[top_row_ci][ci] = current_symbol; /* if game_copy->currentPlayer == 1 then put 'X', otherwise put 'O' */
				game_copy->tops[ci] = game_copy->tops[ci] + 1; /* increasing tops[ci] by 1 */
				int game_value = evaluate_board(game_copy); /* evaluating the board */
				update_best_move(&best_move, game_value, ci, game_copy->currentPlayer); /* update best move, if necessary */
				game_copy->gameBoard[top_row_ci][ci] = ' '; /* restoring game_copy->gameBoard[top_ci][i] to be empty again */
				game_copy->tops[ci] = game_copy->tops[ci] - 1; /* decreasing tops[ci] by 1 */
			}
		}
	} else { /* maxDepth > 1, need for recursion calls */
		for (int move_ci = 0; move_ci < SP_FIAR_GAME_N_COLUMNS; move_ci++) {
			move_value next_move;
			int top_row_ci = game_copy->tops[move_ci];
			if (top_row_ci < SP_FIAR_GAME_N_ROWS) { /* column isn't full */
				game_copy->gameBoard[top_row_ci][move_ci] = current_symbol; /* if game_copy->currentPlayer == 1 then put 'X', otherwise put 'O'*/
				game_copy->tops[move_ci] = game_copy->tops[move_ci] + 1; /* increasing tops[move_ci] */
				game_copy->currentPlayer = (game_copy->currentPlayer == '1' ? '2' : '1'); /* changing the turn*/
				next_move = minimaxAlgo(game_copy, maxDepth - 1); /* recursive call */
				game_copy->currentPlayer = (game_copy->currentPlayer == '1' ? '2' : '1'); /* changing the turn back */
				update_best_move(&best_move, next_move.value, move_ci, game_copy->currentPlayer); /* update best-move, if necessary */
				game_copy->tops[move_ci] = game_copy->tops[move_ci] - 1; /* decreasing tops[move_ci] */
				game_copy->gameBoard[top_row_ci][move_ci] = ' '; /* restoring game_copy->gameBoard[top_ci][i] to be empty again */
				if ((next_move.move == -2 ) && (best_move.value == INT_MIN || best_move.value == INT_MAX)) { /* this was probably a winning move! */
					return best_move; /* no need for further evaluations, found the best move - somebody probably just won the game! */
				}
			}
		}
	}
	return best_move;
}

void check_eval1() {
	SPFiarGame* game = spFiarGameCreate(10);
	char symbol1 = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	char symbol2 = SP_FIAR_GAME_PLAYER_2_SYMBOL;
	game->gameBoard[0][1] = symbol1;
	game->gameBoard[1][1] = symbol1;
	game->gameBoard[2][1] = symbol2;
	game->gameBoard[3][1] = symbol1;
	game->gameBoard[4][1] = symbol1;

	game->gameBoard[0][2] = symbol1;
	game->gameBoard[1][2] = symbol2;
	game->gameBoard[2][2] = symbol2;

	game->gameBoard[0][3] = symbol2;
	game->gameBoard[1][3] = symbol2;
	game->gameBoard[2][3] = symbol1;

	game->gameBoard[0][4] = symbol1;

	game->gameBoard[0][5] = symbol2;

	game->gameBoard[0][6] = symbol2;


	int evaluation = evaluate_board(game);
	spFiarGamePrintBoard(game);
	printf("value of the board: %d", evaluation);
	spFiarGameDestroy(game);

}

void check_eval2() {  /* should be victory for player 1 */
	SPFiarGame* game = spFiarGameCreate(10);
	char symbol1 = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	char symbol2 = SP_FIAR_GAME_PLAYER_2_SYMBOL;

	game->gameBoard[0][2] = symbol1;
	game->gameBoard[1][2] = symbol1;
	game->gameBoard[2][2] = symbol1;

	game->gameBoard[0][3] = symbol2;
	game->gameBoard[1][3] = symbol2;
	game->gameBoard[2][3] = symbol2;

	game->gameBoard[0][4] = symbol1;
	game->gameBoard[1][4] = symbol1;
	game->gameBoard[2][4] = symbol1;

	int evaluation = evaluate_board(game);
	spFiarGamePrintBoard(game);
	printf("value of the board: %d", evaluation);
	spFiarGameDestroy(game);
}

void check_eval3() {  /* should be victory for player 1 */
	SPFiarGame* game = spFiarGameCreate(10);
	char symbol1 = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	//char symbol2 = SP_FIAR_GAME_PLAYER_2_SYMBOL;

	game->gameBoard[0][6] = symbol1;
	game->gameBoard[1][6] = symbol1;
	game->gameBoard[2][6] = symbol1;


	int evaluation = evaluate_board(game);
	spFiarGamePrintBoard(game);
	printf("value of the board: %d", evaluation);
	spFiarGameDestroy(game);
}

void check_eval_with_uri1() {
	SPFiarGame* game = spFiarGameCreate(10);
	char symbol1 = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	char symbol2 = SP_FIAR_GAME_PLAYER_2_SYMBOL;

	/* first row */
	game->gameBoard[0][0] = symbol1;
	game->gameBoard[0][1] = symbol1;
	game->gameBoard[0][2] = symbol1;
	game->gameBoard[0][3] = symbol2;
	game->gameBoard[0][4] = symbol2;
	game->gameBoard[0][6] = symbol2;

	/* second row */
	game->gameBoard[1][1] = symbol2;
	game->gameBoard[1][2] = symbol2;
	game->gameBoard[1][3] = symbol1;
	game->gameBoard[1][4] = symbol1;

	/* thrid row */
	game->gameBoard[2][2] = symbol2;
	game->gameBoard[2][3] = symbol2;
	game->gameBoard[2][4] = symbol1;

	/* forth row */
	game->gameBoard[3][2] = symbol1;
	game->gameBoard[3][3] = symbol1;
	game->gameBoard[3][4] = symbol2;

	int evaluation = evaluate_board(game);
	spFiarGamePrintBoard(game);
	printf("value of the board: %d\n", evaluation);
	fflush(stdout);
	spFiarGameDestroy(game);
}

void check_eval_with_uri2() {
	SPFiarGame* game = spFiarGameCreate(10);
	char symbol1 = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	char symbol2 = SP_FIAR_GAME_PLAYER_2_SYMBOL;

	/* first row */
	game->gameBoard[0][0] = symbol1;
	game->gameBoard[0][1] = symbol2;
	game->gameBoard[0][2] = symbol2;
	game->gameBoard[0][3] = symbol1;
	game->gameBoard[0][4] = symbol2;
	game->gameBoard[0][5] = symbol2;
	game->gameBoard[0][6] = symbol2;

	/* second row */
	game->gameBoard[1][0] = symbol2;
	game->gameBoard[1][1] = symbol1;
	game->gameBoard[1][2] = symbol2;
	game->gameBoard[1][3] = symbol1;
	game->gameBoard[1][4] = symbol1;
	game->gameBoard[1][5] = symbol2;

	/* thrid row */
	game->gameBoard[2][0] = symbol1;
	game->gameBoard[2][1] = symbol1;
	game->gameBoard[2][2] = symbol2;
	game->gameBoard[2][3] = symbol1;
	game->gameBoard[2][4] = symbol1;

	/* forth row */
	game->gameBoard[3][0] = symbol2;
	game->gameBoard[3][1] = symbol2;
	game->gameBoard[3][2] = symbol1;
	game->gameBoard[3][3] = symbol2;

	/* fifth row */
	game->gameBoard[4][0] = symbol1;
	game->gameBoard[4][1] = symbol2;
	game->gameBoard[4][2] = symbol2;

	/* sixth row */
	game->gameBoard[5][0] = symbol2;
	game->gameBoard[5][1] = symbol2;

	int evaluation = evaluate_board(game);
	spFiarGamePrintBoard(game);
	printf("value of the board: %d\n", evaluation);
	fflush(stdout);
	spFiarGameDestroy(game);
}

void check_eval_with_uri3() {
	SPFiarGame* game = spFiarGameCreate(10);
	char symbol1 = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	char symbol2 = SP_FIAR_GAME_PLAYER_2_SYMBOL;

	/* first row */
	game->gameBoard[0][0] = symbol2;
	game->gameBoard[0][1] = symbol2;
	game->gameBoard[0][2] = symbol2;
	game->gameBoard[0][3] = symbol1;
	game->gameBoard[0][4] = symbol2;
	game->gameBoard[0][5] = symbol1;
	game->gameBoard[0][6] = symbol1;

	/* second row */
	game->gameBoard[1][0] = symbol2;
	game->gameBoard[1][1] = symbol2;
	game->gameBoard[1][2] = symbol2;
	game->gameBoard[1][3] = symbol1;
	game->gameBoard[1][4] = symbol2;
	game->gameBoard[1][5] = symbol1;
	game->gameBoard[1][6] = symbol2;

	/* thrid row */
	game->gameBoard[2][2] = symbol1;
	game->gameBoard[2][3] = symbol2;
	game->gameBoard[2][4] = symbol1;

	/* forth row */
	game->gameBoard[3][2] = symbol1;
	game->gameBoard[3][3] = symbol2;


	int evaluation = evaluate_board(game);
	spFiarGamePrintBoard(game);
	printf("value of the board: %d\n", evaluation);
	fflush(stdout);
	spFiarGameDestroy(game);
}

void check_eval_with_uri4() {
	SPFiarGame* game = spFiarGameCreate(10);
	char symbol1 = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	char symbol2 = SP_FIAR_GAME_PLAYER_2_SYMBOL;

	/* first row */
	game->gameBoard[0][0] = symbol2;

	game->gameBoard[0][6] = symbol1;

	/* second row */
	game->gameBoard[1][0] = symbol2;
	game->gameBoard[1][1] = symbol2;
	game->gameBoard[1][4] = symbol2;
	game->gameBoard[1][6] = symbol2;

	/* thrid row */
	game->gameBoard[2][2] = symbol1;
	game->gameBoard[2][3] = symbol2;
	game->gameBoard[2][4] = symbol1;

	/* forth row */
	game->gameBoard[3][2] = symbol1;
	game->gameBoard[3][3] = symbol2;

	/* corners */
	game->gameBoard[5][0] = symbol1;
	game->gameBoard[5][6] = symbol1;



	int evaluation = evaluate_board(game);
	spFiarGamePrintBoard(game);
	printf("value of the board: %d\n", evaluation);
	fflush(stdout);
	spFiarGameDestroy(game);
}

void check_eval_with_uri5() {
	SPFiarGame* game = spFiarGameCreate(10);
	char symbol1 = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	char symbol2 = SP_FIAR_GAME_PLAYER_2_SYMBOL;

	/* second row */
	game->gameBoard[1][1] = symbol2;
	game->gameBoard[1][4] = symbol1;
	game->gameBoard[1][6] = symbol2;

	/* forth row */
	game->gameBoard[3][2] = symbol1;

	/* corners */
	game->gameBoard[5][0] = symbol1;
	game->gameBoard[5][6] = symbol1;



	int evaluation = evaluate_board(game);
	spFiarGamePrintBoard(game);
	printf("value of the board: %d\n", evaluation);
	fflush(stdout);
	spFiarGameDestroy(game);
}

void check_eval_with_uri6() {
	SPFiarGame* game = spFiarGameCreate(10);
	char symbol1 = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	//char symbol2 = SP_FIAR_GAME_PLAYER_2_SYMBOL;

	/* corners */
	game->gameBoard[5][0] = symbol1;
	game->gameBoard[5][6] = symbol1;



	int evaluation = evaluate_board(game);
	spFiarGamePrintBoard(game);
	printf("value of the board: %d\n", evaluation);
	fflush(stdout);
	spFiarGameDestroy(game);
}

void tie_board() {

	SPFiarGame* game = spFiarGameCreate(10);
	char symbol1 = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	char symbol2 = SP_FIAR_GAME_PLAYER_2_SYMBOL;
	char game_status;

	game->tops[0] = 6;
	game->tops[1] = 6;
	game->tops[2] = 6;
	game->tops[3] = 6;
	game->tops[4] = 6;
	game->tops[5] = 6;
	game->tops[6] = 6;

	/* first row */
	game->gameBoard[0][0] = symbol2;
	game->gameBoard[0][1] = symbol1;
	game->gameBoard[0][2] = symbol2;
	game->gameBoard[0][3] = symbol1;
	game->gameBoard[0][4] = symbol2;
	game->gameBoard[0][5] = symbol1;
	game->gameBoard[0][6] = symbol2;

	/* second row */
	game->gameBoard[1][0] = symbol1;
	game->gameBoard[1][1] = symbol2;
	game->gameBoard[1][2] = symbol1;
	game->gameBoard[1][3] = symbol2;
	game->gameBoard[1][4] = symbol1;
	game->gameBoard[1][5] = symbol2;
	game->gameBoard[1][6] = symbol1;

	/* third row */
	game->gameBoard[2][0] = symbol1;
	game->gameBoard[2][1] = symbol2;
	game->gameBoard[2][2] = symbol1;
	game->gameBoard[2][3] = symbol2;
	game->gameBoard[2][4] = symbol1;
	game->gameBoard[2][5] = symbol2;
	game->gameBoard[2][6] = symbol1;

	/* forth row */
	game->gameBoard[3][0] = symbol2;
	game->gameBoard[3][1] = symbol1;
	game->gameBoard[3][2] = symbol2;
	game->gameBoard[3][3] = symbol1;
	game->gameBoard[3][4] = symbol2;
	game->gameBoard[3][5] = symbol1;
	game->gameBoard[3][6] = symbol2;

	/* fifth row */
	game->gameBoard[4][0] = symbol1;
	game->gameBoard[4][1] = symbol2;
	game->gameBoard[4][2] = symbol1;
	game->gameBoard[4][3] = symbol2;
	game->gameBoard[4][4] = symbol1;
	game->gameBoard[4][5] = symbol2;
	game->gameBoard[4][6] = symbol1;

	/* fifth row */
	game->gameBoard[5][0] = symbol1;
	game->gameBoard[5][1] = symbol2;
	game->gameBoard[5][2] = symbol1;
	game->gameBoard[5][3] = symbol2;
	game->gameBoard[5][4] = symbol1;
	game->gameBoard[5][5] = symbol2;
	game->gameBoard[5][6] = symbol1;

	int evaluation = evaluate_board(game);
	spFiarGamePrintBoard(game);
	printf("value of the board: %d\n", evaluation);
	fflush(stdout);
	game_status = spFiarCheckWinner(game);
	printf("game-status: %c\n", game_status);

	spFiarGameDestroy(game);
}

void losing_boards() {
	move_value fake_move = {0, 0};
	SPFiarGame* game = spFiarGameCreate(10);
	char symbol1 = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	char symbol2 = SP_FIAR_GAME_PLAYER_2_SYMBOL;
	char game_status;
	/* winning in a row */
	game->gameBoard[5][0] = symbol1;
	game->gameBoard[5][1] = symbol1;
	game->gameBoard[5][2] = symbol1;
	game->gameBoard[5][3] = symbol1;

	int evaluation = evaluate_board(game);

	game_status = spFiarCheckWinner(game);
	is_the_game_over(game, &fake_move);
	printf("winner: %c, value of the board: %d, fake-move.value: %d\n", game_status, evaluation, fake_move.value);
	fake_move.value = 0;

	fflush(stdout);
	spFiarGameDestroy(game);

	/* winning in a column */
	game = spFiarGameCreate(10);
	game->gameBoard[1][0] = symbol1;
	game->gameBoard[2][0] = symbol1;
	game->gameBoard[3][0] = symbol1;
	game->gameBoard[4][0] = symbol1;

	evaluation = evaluate_board(game);
	spFiarGamePrintBoard(game);
	game_status = spFiarCheckWinner(game);
	is_the_game_over(game, &fake_move);
	printf("winner: %c, value of the board: %d, fake-move.value: %d\n", game_status, evaluation, fake_move.value);
	fake_move.value = 0;
	fflush(stdout);
	spFiarGameDestroy(game);

	/* winning in a diagonal 1 */
	game = spFiarGameCreate(10);
	game->gameBoard[5][0] = symbol1;
	game->gameBoard[4][1] = symbol1;
	game->gameBoard[3][2] = symbol1;
	game->gameBoard[2][3] = symbol1;

	evaluation = evaluate_board(game);
	spFiarGamePrintBoard(game);
	game_status = spFiarCheckWinner(game);
	is_the_game_over(game, &fake_move);
	printf("winner: %c, value of the board: %d, fake-move.value: %d\n", game_status, evaluation, fake_move.value);
	fake_move.value = 0;
	fflush(stdout);
	spFiarGameDestroy(game);

	/* winning in a diagonal 2 */
	game = spFiarGameCreate(10);
	game->gameBoard[3][0] = symbol1;
	game->gameBoard[2][1] = symbol1;
	game->gameBoard[1][2] = symbol1;
	game->gameBoard[0][3] = symbol1;

	evaluation = evaluate_board(game);
	spFiarGamePrintBoard(game);
	game_status = spFiarCheckWinner(game);
	is_the_game_over(game, &fake_move);
	printf("winner: %c, value of the board: %d, fake-move.value: %d\n", game_status, evaluation, fake_move.value);
	fake_move.value = 0;
	fflush(stdout);
	spFiarGameDestroy(game);

	/* winning in a diagonal 3 */
	game = spFiarGameCreate(10);
	game->gameBoard[3][2] = symbol2;
	game->gameBoard[2][3] = symbol2;
	game->gameBoard[1][4] = symbol2;
	game->gameBoard[0][5] = symbol2;

	evaluation = evaluate_board(game);
	spFiarGamePrintBoard(game);
	game_status = spFiarCheckWinner(game);
	is_the_game_over(game, &fake_move);
	printf("winner: %c, value of the board: %d, fake-move.value: %d\n", game_status, evaluation, fake_move.value);
	fake_move.value = 0;
	fflush(stdout);

	/* not a winnig board */

	game = spFiarGameCreate(10);
	game->gameBoard[3][2] = symbol2;
	game->gameBoard[2][3] = symbol2;
	game->gameBoard[1][4] = symbol2;
	evaluation = evaluate_board(game);
	spFiarGamePrintBoard(game);
	game_status = spFiarCheckWinner(game);
	is_the_game_over(game, &fake_move);
	printf("winner: %c, value of the board: %d, fake-move.value: %d\n", game_status, evaluation, fake_move.value);
	fake_move.value = 0;
	spFiarGameDestroy(game);
	fflush(stdout);
}

void check_scores() {
	check_eval1();
	printf("\n");
	fflush(stdout);
	check_eval3();
	printf("\n");
	fflush(stdout);

	check_eval_with_uri1();
	printf("\n");
	fflush(stdout);

	check_eval_with_uri2();
	printf("\n");
	fflush(stdout);

	check_eval_with_uri3();
	printf("\n");
	fflush(stdout);

	check_eval_with_uri4();
	printf("\n");
	fflush(stdout);

	check_eval_with_uri5();
	printf("\n");
	fflush(stdout);

	check_eval_with_uri6();

}

void check_eval() {
	losing_boards();
	tie_board();
	fflush(stdout);
}
