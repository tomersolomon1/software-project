/*
 * SPMiniMaxNode.c
 *
 */

#include "SPFIARGame.h"
#include <limits.h>
#include <stddef.h>


typedef struct move_value_t {
	int move; /* the column */
	int value; /* the value of the move, according to the mini-max algo */
} move_value;

move_value minimaxAlgo(SPFiarGame* game_copy, unsigned int maxDepth);

int inner_product(int v[], int u[]) {
	int product = 0;
	for (int i = 0; i < 7; i++) {
		product += v[i] * u[i];
	}
	return product;
}

int winning_value(int winner) {
	int value;
	value = (winner == 1 ? INT_MAX : INT_MIN);
	return value;
}

/* assuming symbol is either the symbol of player 1 ot player 2 */
void update_sequences_length(char symbol, int *player1_seq, int *player2_seq) {
	if (symbol == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
		*player1_seq += 1;
	} else if (symbol == SP_FIAR_GAME_PLAYER_2_SYMBOL) {
		 *player2_seq += 1;
	}
}

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

/* compute sequences in row - to the right
 * returning 0 means no winner, 1 means player 1 won, 2 means player 2 won
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

/* compute sequences in column - up-ward
 * returning 0 means no winner, 1 means player 1 won, 2 means player 2 won
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

/* compute sequences in diagonal - up-right-ward
 * returning 0 means no winner, 1 means player 1 won, 2 means player 2 won
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

/* compute sequences in diagonal - up-left
 * returning 0 means no winner, 1 means player 1 won, 2 means player 2 won
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

/*
 * the algorithm: calculating sequences of player 1 & 2, and then sum it up with the weights vector [-5, -2, -1, 0, 1, 2, 5]
 * operates in O(n) complexity
 */
int evaluate_board(SPFiarGame* currentGame) {
	int sequences_counter[7] = { 0 };
	int weights[7] = {-5, -2, -1, 0, 1, 2, 5};
	int somebody_won = 0; /* somebody_won = 1 means player 1 won, somebody_won = 2 means player 2 won */
	for (int ri = 0; ri < SP_FIAR_GAME_N_ROWS; ri++) {
		for (int cj = 0; cj < SP_FIAR_GAME_N_COLUMNS; cj++) {
			somebody_won = span_diagonal_upleft(currentGame, ri, cj, sequences_counter);
			if (somebody_won != 0) {
				return winning_value(somebody_won);
			}
			somebody_won = span_diagonal_upright(currentGame, ri, cj, sequences_counter);
			if (somebody_won != 0) {
				return winning_value(somebody_won);
			}
			somebody_won = span_row_right(currentGame, ri, cj, sequences_counter);
			if (somebody_won != 0) {
				return winning_value(somebody_won);
			}
			somebody_won = span_column_up(currentGame, ri, cj, sequences_counter);
			if (somebody_won != 0) {
				return winning_value(somebody_won);
			}
		}
	}
	return inner_product(sequences_counter, weights); /* if we got here it means no one has won so far */
}

/* if best_move_so_far->move == -1 updated anyway */
void update_best_move(move_value* best_move_so_far, int value, int move, char current_player) {
	// printf("update move - checking in, current-player: %c, best-so-far-move_value: %d\n", current_player, best_move_so_far->value );
	// fflush(stdout);
	if ((current_player == '1' && value > best_move_so_far->value) || (current_player == '2' && value < best_move_so_far->value)
			|| (best_move_so_far->move == -1)) {
		// printf("updated the move: new-best-move: %d, new-move-value: %d\n", move, value);
		// fflush(stdout);
		best_move_so_far->value = value;
		best_move_so_far->move = move;
	}
}

/* checks if the game is over, and updates this
 * return 1 if the game is over
 * otherwise, return 0
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
		this_move = 0;
		return 1;
	}
	return 0; /* otherwise, no one has won and it's not a tie */
}

char get_symbol(char current_player) {
	char symbol = (current_player == '1' ? SP_FIAR_GAME_PLAYER_1_SYMBOL : SP_FIAR_GAME_PLAYER_2_SYMBOL);
	return symbol;
}

/* assuming maxDepth >= 1
 * the object of player 1 is to maximize the value-function, and the object of player 2 is to minimize the value-function
 * by default, |this_move.value| < INT_MAX, since value = INT_MAX is means the winning of player 1 (similarly, INT_MIN means the winning of player 2) */
move_value minimaxAlgo(SPFiarGame* game_copy, unsigned int maxDepth) {
	// printf("mini-max algo!, max-depth: %d\n", maxDepth);
	// fflush(stdout);
	// spFiarGamePrintBoard(game_copy);
	// fflush(stdout);
	char current_symbol =  get_symbol(game_copy->currentPlayer); /* the symbol of the current player */
	move_value best_move; /* will contain the best move for the player and it's value */
	best_move.move = -1; /* default value */
	best_move.value = (game_copy->currentPlayer == '1' ? (INT_MIN + 1) : (INT_MAX - 1)); /* default value for the mini-max algo */
	// printf("checking if the game is over\n");
	fflush(stdout);
	if (is_the_game_over(game_copy, &best_move)) { /* checking if the game is over, and if so - this_move.move will be -1 */
		// printf("the game is over indid\n");
		// fflush(stdout);
		return best_move;
	}

	if (maxDepth == 1) { /* halting condition */
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
				game_copy->tops[move_ci] = game_copy->tops[move_ci] + 1; /* increasing tops[ci] */
				game_copy->currentPlayer = (game_copy->currentPlayer == '1' ? '2' : '1'); /* changing the turn*/

				next_move = minimaxAlgo(game_copy, maxDepth - 1);

				game_copy->currentPlayer = (game_copy->currentPlayer == '1' ? '2' : '1'); /* changing the turn back */
				/* update best move, if necessary - if this move just won the the game or made it a tie it will the recursion will fold-back immediately  */
				update_best_move(&best_move, next_move.value, move_ci, game_copy->currentPlayer);
				game_copy->tops[move_ci] = game_copy->tops[move_ci] - 1; /* decreasing tops[ci] */
				game_copy->gameBoard[top_row_ci][move_ci] = ' '; /* restoring game_copy->gameBoard[top_ci][i] to be empty again */
				if ((next_move.move == -1) && (next_move.value == INT_MIN || next_move.value == INT_MAX)) { /* this was probably a winning move! */
					break; /* no need for further evaluations, found the best move - somebody has just won the game! */
				}
			}
		}

	}
	// printf("mini-max algo - good-bye, max-depth: %d, best-move: %d, best-move-value: %d\n", maxDepth, best_move.move, best_move.value);
	fflush(stdout);
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

void check_eval() {
	//check_eval1();
	check_eval3();
}
