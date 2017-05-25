/*
 * SPMiniMaxNode.c
 *
 *  Created on: 25 במאי 2017
 *      Author: User
 */

#include "SPFIARGame.h"
#include <limits.h>
#include <stddef.h>

typedef struct move_consequences_t {
	int column;
	int value;
} move_consequences;

SPFiarGame *create_board() {
	SPFiarGame* new_board = (SPFiarGame *) malloc(sizeof(SPFiarGame));
	for (int i = 0; i < 6; i++) {
		for(int j = 0; j < 7; j++) {
			new_board->gameBoard[i][j] = 2; /* defualt value, means empty */
		}
	for (int ci = 0 ; ci < 7; ci++) {
		new_board->tops[ci];
	}
	return new_board;
}

/* creating another copy of SPFiarGame */
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
		move_consequences best_move  = minimaxAlgo(game_copy);
		if (best_move.column != -1) {
			return best_move.column;
		} else {
			/* no legal move - should ask the forum */
			return -1;
		}
	} else {
		return -1;
	}
}

void update_best_move(move_consequences* this_move, int value, int ci, char current_player) {
	if ((current_player == '1' && value > this_move->value) || (current_player == '2' && value < this_move->value)) {
			this_move->value = value;
			this_move->column = ci;
	}
}

/* assuming maxDepth >= 1*/
move_consequences minimaxAlgo(SPFiarGame* game_copy, unsigned int maxDepth) {
	char current_symbol;
	move_consequences this_move;
	game_copy->currentPlayer == '1' ? current_symbol = SP_FIAR_GAME_PLAYER_1_SYMBOL : current_symbol = SP_FIAR_GAME_PLAYER_2_SYMBOL;
	this_move.column = -1; /* default value */
	game_copy->currentPlayer == '1' ? this_move.value = INT_MIN : this_move.value = INT_MAX;
	if (is_board_ful(game_copy)) {
		return this_move;
	}

	if (maxDepth == 1) { /* halting condition */
		for (int ci = 0; ci < SP_FIAR_GAME_N_COLUMNS; ci++) {
			int top_row_ci = game_copy->tops[ci];
			if (top_row_ci < SP_FIAR_GAME_N_ROWS) { /* column isn't full */
				game_copy->gameBoard[top_row_ci][ci] = current_symbol; /* if game_copy->currentPlayer == 1 then put 'X', otherwise put 'O'*/
				int game_value = evaluate_board(game_copy);
				update_best_move(&this_move, game_value, ci, game_copy->currentPlayer); /* update best move, if necessary */
				game_copy->gameBoard[top_row_ci][ci] = ' '; /* restoring game_copy->gameBoard[top_ci][i] to be empty again */
			}
		}
		return move_consequences;
	} else { /* maxDepth > 1 */
		game_copy->currentPlayer == '1' ? game_copy->currentPlayer = '2' : game_copy->currentPlayer = '1'; /* changing the turn*/
		for (int ci = 0; ci < SP_FIAR_GAME_N_COLUMNS; ci++) {
			move_consequences next_move;
			int top_row_ci = game_copy->tops[ci];
			if (top_row_ci < SP_FIAR_GAME_N_ROWS) { /* column isn't full */
				game_copy->gameBoard[top_row_ci][ci] = current_symbol; /* if game_copy->currentPlayer == 1 then put 'X', otherwise put 'O'*/
				next_move = minimaxAlgo(game_copy, maxDepth - 1);
				update_best_move(&this_move, next_move.value, ci, game_copy->currentPlayer); /* update best move, if necessary */
				game_copy->gameBoard[top_row_ci][ci] = ' '; /* restoring game_copy->gameBoard[top_ci][i] to be empty again */
			}
		}
	}
	return this_move;
}



