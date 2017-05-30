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

SPFiarGame *create_board() {
	SPFiarGame* new_board = (SPFiarGame *) malloc(sizeof(SPFiarGame));
	for (int i = 0; i < 6; i++) {
		for(int j = 0; j < 7; j++) {
			new_board->gameBoard[i][j] = 2; /* defualt value, means empty */
		}
	}
	for (int ci = 0 ; ci < 7; ci++) {
		new_board->tops[ci];
	}
	return new_board;
}

/* creating another copy of SPFiarGame - for MiniMax algo */
SPFiarGame* copy_board(SPFiarGame* game) {
	SPFiarGame* new_game = (SPFiarGame *) malloc(sizeof(SPFiarGame));
	if (new_game == NULL) {
		return NULL;
	} else {
		for (int ri = 0; ri < SP_FIAR_GAME_N_ROWS; ri++) {
				for (int cj = 0; cj < SP_FIAR_GAME_N_COLUMNS; cj++) {
					new_game->gameBoard[ri][cj] = game->gameBoard[ri][cj];
				}
			}
			for (int ci = 0; ci < SP_FIAR_GAME_N_COLUMNS; ci++) {
				new_game->tops[ci] = game->tops[ci];
			}
			new_game->currentPlayer = game->currentPlayer;
			return new_game;
	}
}


int spMinimaxSuggestMove(SPFiarGame* currentGame, unsigned int maxDepth) {
	SPFiarGame* game_copy = copy_board(currentGame);
	if (game_copy != NULL) {
		//if (game_is_not_over) - should implement
		move_value best_move  = minimaxAlgo(game_copy);
		if (best_move.move != -1) {
			return best_move.move;
		} else {
			/* no legal move - should ask the forum  */
			/* according to the return value we'll be able to who won / there was a tie */
			return -1;
		}
	} else { /* memory problem */
		return -2;
	}
}

void update_best_move(move_value* this_move, int value, int ci, char current_player) {
	if ((current_player == '1' && value > this_move->value) || (current_player == '2' && value < this_move->value)) {
			this_move->value = value;
			this_move->move = ci;
	}
}


/* checks if the game is over, and updates this
 * return 1 if the game is over
 * otherwise, return 0 */
int is_game_over(SPFiarGame* game_copy, move_value* this_move) {
	char game_status = spFiarCheckWinner(game_copy);
	if (game_status == SP_FIAR_GAME_PLAYER_1_SYMBOL) {

	}
	return 1;
}




/* assuming maxDepth >= 1*/
move_value minimaxAlgo(SPFiarGame* game_copy, unsigned int maxDepth) {
	char current_symbol; /* the symbol of the current player */
	move_value this_move; /* will contain the best move and it's value */
	game_copy->currentPlayer == '1' ? current_symbol = SP_FIAR_GAME_PLAYER_1_SYMBOL : current_symbol = SP_FIAR_GAME_PLAYER_2_SYMBOL;
	this_move.move = -1; /* default value */
	game_copy->currentPlayer == '1' ? this_move.value = INT_MIN : this_move.value = INT_MAX; /* defualt value for the mini-max algo */
	if (is_game_over(game_copy, &this_move)) { /* checking if the game is over */
		return this_move;
	}

	if (maxDepth == 1) { /* halting condition */
		for (int ci = 0; ci < SP_FIAR_GAME_N_COLUMNS; ci++) {
			int top_row_ci = game_copy->tops[ci]; /* to be changed to is-valid-move */
			if (top_row_ci < SP_FIAR_GAME_N_ROWS) { /* column isn't full */
				game_copy->gameBoard[top_row_ci][ci] = current_symbol; /* if game_copy->currentPlayer == 1 then put 'X', otherwise put 'O'*/
				game_copy->tops[ci] = game_copy->tops[ci] + 1; /* increasing tops[ci] */
				int game_value = evaluate_board(game_copy);
				update_best_move(&this_move, game_value, ci, game_copy->currentPlayer); /* update best move, if necessary */
				game_copy->gameBoard[top_row_ci][ci] = ' '; /* restoring game_copy->gameBoard[top_ci][i] to be empty again */
				game_copy->tops[ci] = game_copy->tops[ci] - 1; /* decreasing tops[ci] */
			}
		}
		return move_value;
	} else { /* maxDepth > 1, need for recursion */
		game_copy->currentPlayer == '1' ? game_copy->currentPlayer = '2' : game_copy->currentPlayer = '1'; /* changing the turn*/
		for (int ci = 0; ci < SP_FIAR_GAME_N_COLUMNS; ci++) {
			move_value next_move;
			int top_row_ci = game_copy->tops[ci];
			if (top_row_ci < SP_FIAR_GAME_N_ROWS) { /* column isn't full */
				game_copy->gameBoard[top_row_ci][ci] = current_symbol; /* if game_copy->currentPlayer == 1 then put 'X', otherwise put 'O'*/
				game_copy->tops[ci] = game_copy->tops[ci] + 1; /* increasing tops[ci] */
				next_move = minimaxAlgo(game_copy, maxDepth - 1);
				update_best_move(&this_move, next_move.value, ci, game_copy->currentPlayer); /* update best move, if necessary */
				game_copy->tops[ci] = game_copy->tops[ci] - 1; /* decreasing tops[ci] */
				game_copy->gameBoard[top_row_ci][ci] = ' '; /* restoring game_copy->gameBoard[top_ci][i] to be empty again */
				if ((next_move.move == -1) && (next_move.value == INT_MIN || next_move.value == INT_MAX)) { /* this was a winning move! */
					break; /* no need for further evaluations, found the best move - somebody has just won the game! */
				}
			}
		}
		game_copy->currentPlayer == '1' ? game_copy->currentPlayer = '2' : game_copy->currentPlayer = '1'; /* changing the turn back */
	}
	return this_move;
}
