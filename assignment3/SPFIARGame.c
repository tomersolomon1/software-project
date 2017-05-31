
/**
 * SPFIARGame Summary:
 *
 * A container that represents a classic connect-4 game, a two players 6 by 7
 * board game (rows X columns). The container supports the following functions.
 *
 * spFiarGameCreate           - Creates a new game board
 * spFiarGameCopy             - Copies a game board
 * spFiarGameDestroy          - Frees all memory resources associated with a game
 * spFiarGameSetMove          - Sets a move on a game board
 * spFiarGameIsValidMove      - Checks if a move is valid
 * spFiarGameUndoPrevMove     - Undoes previous move made by the last player
 * spFiarGamePrintBoard       - Prints the current board
 * spFiarGameGetCurrentPlayer - Returns the current player
 *
 */

//Definitions
#define SP_FIAR_GAME_SPAN 4
#define SP_FIAR_GAME_N_ROWS 6
#define SP_FIAR_GAME_N_COLUMNS 7
#define SP_FIAR_GAME_PLAYER_1_SYMBOL 'X'
#define SP_FIAR_GAME_PLAYER_2_SYMBOL 'O'
#define SP_FIAR_GAME_TIE_SYMBOL '-'
#define SP_FIAR_GAME_EMPTY_ENTRY ' '
#define Player1 '1'
#define Player2 '2'

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "SPArrayList.h"
#include "SPFIARGame.h"


/**
 * Creates a new game with a specified history size. The history size is a
 * parameter which specifies the number of previous moves to store. If the number
 * of moves played so far exceeds this parameter, then first moves stored will
 * be discarded in order for new moves to be stored.
 *
 * @historySize - The total number of moves to undo,
 *                a player can undo at most historySizeMoves turns.
 * @return
 * NULL if either a memory allocation failure occurs or historySize <= 0.
 * Otherwise, a new game instant is returned.
 */
SPFiarGame* spFiarGameCreate(int historySize){
	SPFiarGame* newGame = (SPFiarGame*) malloc(sizeof(SPFiarGame));
	if(newGame == NULL){
		return NULL;
	}
	newGame->currentPlayer = Player1;
	for (int i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++){
		newGame->tops[i] = 0;
	}
	for(int i = 0; i < SP_FIAR_GAME_N_ROWS; i++){
		for(int j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++){
			newGame->gameBoard[i][j] = SP_FIAR_GAME_EMPTY_ENTRY;
		}
	}
	SPArrayList* history1 = spArrayListCreate(historySize);
	newGame->history = history1;
	return newGame;
}

/**
 *	Creates a copy of a given game.
 *	The new copy has the same status as the src game.
 *
 *	@param src - the source game which will be copied
 *	@return
 *	NULL if either src is NULL or a memory allocation failure occurred.
 *	Otherwise, an new copy of the source game is returned.
 *
 */
SPFiarGame* spFiarGameCopy(SPFiarGame* src){
	if(src == NULL){
		return NULL;
	}
	int history_size = spArrayListMaxCapacity(src->history);
	SPFiarGame* dst = spFiarGameCreate(history_size);
	if(dst == NULL){
		return NULL;
	}
	dst->currentPlayer = src->currentPlayer;
	for(int i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++){
		dst->tops[i] = (src->tops)[i];
	}
	for(int i = 0; i < SP_FIAR_GAME_N_ROWS; i++){
		for(int j = 0; i < SP_FIAR_GAME_N_COLUMNS; j++){
			dst->gameBoard[i][j] = (src->gameBoard)[i][j];
		}
	}
	SPArrayList* history  = spArrayListCopy(src->history);
	if(history == NULL){
		return NULL;
	}
	dst->history = history;
	return dst;
}

/**
 * Frees all memory allocation associated with a given game. If src==NULL
 * the function does nothing.
 *
 * @param src - the source game
 */
void spFiarGameDestroy(SPFiarGame* src){
	if(src != NULL){
		spArrayListDestroy(src->history);
		free(src);
	}
}

/**
 * Sets the next move in a given game by specifying column index. The
 * columns are 0-based and in the range [0,SP_FIAR_GAME_N_COLUMNS -1].
 *
 * @param src - The target game
 * @param col - The target column, the columns are 0-based
 * @return
 * SP_FIAR_GAME_INVALID_ARGUMENT - if src is NULL or col is out-of-range
 * SP_FIAR_GAME_INVALID_MOVE - if the given column is full.
 */
SP_FIAR_GAME_MESSAGE spFiarGameSetMove(SPFiarGame* src, int col){
	if(src == NULL){
		return SP_FIAR_GAME_INVALID_ARGUMENT;
	}
	if(col < 0 || col > SP_FIAR_GAME_N_COLUMNS - 1){
		return SP_FIAR_GAME_INVALID_ARGUMENT;
	}
	if(spFiarGameIsValidMove(src, col) == false){
		return SP_FIAR_GAME_INVALID_MOVE;
	}
	int row = (src->tops)[col];
	if(src->currentPlayer == Player1){
		(src->gameBoard)[row][col] = SP_FIAR_GAME_PLAYER_1_SYMBOL;
		src->currentPlayer = Player2;
	}
	else if(src->currentPlayer == Player2){
		(src->gameBoard)[row][col] = SP_FIAR_GAME_PLAYER_2_SYMBOL;
		src->currentPlayer = Player1;
	}
	(src->tops)[col]++;
	spArrayListPushFirst((src->history), col);
	return SP_FIAR_GAME_SUCCESS;
}

/**
 * Checks if a disk can be put in the specified column.
 *
 * @param src - The source game
 * @param col - The specified column
 * @return
 * true  - if the a disc can be put in the target column
 * false - otherwise.
 */
bool spFiarGameIsValidMove(SPFiarGame* src, int col){
	if(src == NULL){
		return false;
	}
	int top = (src->tops)[col];
	if(0 <= top && top < SP_FIAR_GAME_N_ROWS){
		return true;
	}
	return false;
}

/**
 * Removes a disc that was put in the previous move and changes the current
 * player's turn. If the user invoked this command more than historySize times
 * in a row then an error occurs.
 *
 * @param src - The source game
 * @return
 * SP_FIAR_GAME_INVALID_ARGUMENT - if src == NULL
 * SP_FIAR_GAME_NO_HISTORY       - if the user invoked this function more then
 *                                 historySize in a row.
 * SP_FIAR_GAME_SUCCESS          - on success. The last disc that was put on the
 *                                 board is removed and the current player is changed
 */
SP_FIAR_GAME_MESSAGE spFiarGameUndoPrevMove(SPFiarGame* src){
	if(src == NULL){
		return SP_FIAR_GAME_INVALID_ARGUMENT;
	}
	if(spArrayListSize(src->history) == 0){
		return SP_FIAR_GAME_NO_HISTORY;
	}
	int col_last_move = spArrayListGetFirst(src->history);
	int row_last_move = src->tops[col_last_move] - 1;
	src->gameBoard[row_last_move][col_last_move] = SP_FIAR_GAME_EMPTY_ENTRY;
	src->tops[col_last_move] --;
	if(src->currentPlayer == Player1){
		src->currentPlayer = Player2;
	}
	else if(src->currentPlayer == Player2){
		src->currentPlayer = Player1;
	}
	spArrayListRemoveFirst(src->history);
	return SP_FIAR_GAME_SUCCESS;
}


/**
 * On success, the function prints the board game. If an error occurs, then the
 * function does nothing. The characters 'X' and 'O' are used to represent
 * the discs of player 1 and player 2, respectively.
 *
 * @param src - the target game
 * @return
 * SP_FIAR_GAME_INVALID_ARGUMENT - if src==NULL
 * SP_FIAR_GAME_SUCCESS - otherwise
 *
 */
SP_FIAR_GAME_MESSAGE spFiarGamePrintBoard(SPFiarGame* src){
	if(src == NULL){
		return SP_FIAR_GAME_INVALID_ARGUMENT;
	}
	for(int row = SP_FIAR_GAME_N_ROWS - 1; row >= 0 ; row--){
		printf("| ");
		fflush(stdout);
		for(int col = 0; col < SP_FIAR_GAME_N_COLUMNS; col++){
			printf("%c ", src->gameBoard[row][col]);
			fflush(stdout);
		}
		printf("|\n");
		fflush(stdout);
	}
	for(int i = 0; i < 17; i++){
		printf("-");
		fflush(stdout);
	}
	printf("\n  ");
	fflush(stdout);
	for(int i = 0; i < 7; i++){
		printf("%d ", i + 1);
		fflush(stdout);
	}
	printf("  \n");
	fflush(stdout);
	return SP_FIAR_GAME_SUCCESS;
}

/**
 * Returns the current player of the specified game.
 * @param src - the source game
 * @return
 * SP_FIAR_GAME_PLAYER_1_SYMBOL - if it's player one's turn
 * SP_FIAR_GAME_PLAYER_2_SYMBOL - if it's player two's turn
 * SP_FIAR_GAME_EMPTY_ENTRY     - otherwise
 */
char spFiarGameGetCurrentPlayer(SPFiarGame* src){
	if(src == NULL){
		return SP_FIAR_GAME_EMPTY_ENTRY;
	}
	if(src->currentPlayer == Player1){
		return SP_FIAR_GAME_PLAYER_1_SYMBOL;
	}
	if(src->currentPlayer == Player2){
		return SP_FIAR_GAME_PLAYER_2_SYMBOL;
	}
	return SP_FIAR_GAME_EMPTY_ENTRY;
}


/* return 0 if no 4-in-a-row in this row
 * return 1 if there is 4 in a row - for player 1
 * return 2 if there is 4 in a row - for player 2
 */
int four_in_a_row(SPFiarGame* game_copy, int ri) {
	int sequence_len = 0; /* the length of the current sequence */
	char sequence_symbol = game_copy->gameBoard[ri][0]; /* no player sequence */
	for (int ci = 0; ci < SP_FIAR_GAME_N_COLUMNS; ci++) {
		if (sequence_symbol == game_copy->gameBoard[ri][ci]) {
			if (sequence_symbol != SP_FIAR_GAME_EMPTY_ENTRY) {
				if (sequence_len < 3) { /* it's not 4-in-a-row yet*/
					sequence_len += 1;
				} else { /* found 4-in-a-row! */
					if (sequence_symbol == SP_FIAR_GAME_PLAYER_1_SYMBOL) { /* player 1 won */
						return 1;
					} else { /* player 2 won */
						return 2;
					}
				}
			}
		} else { /* it's a new sequence */
			sequence_symbol = game_copy->gameBoard[ri][ci];
			sequence_len = 0;
		}
	}
	return 0;
}


/* return 0 if no 4-in-a-column in this column
 * return 1 if there is 4 in a column - for player 1
 * return 2 if there is 4 in a column - for player 2
 */

int four_in_a_column(SPFiarGame* game_copy, int ci) {
	int sequence_len = 0; /* the length of the current sequence */
	char sequence_symbol = game_copy->gameBoard[0][ci]; /* no player sequence */
	for (int ri = 0; ri < SP_FIAR_GAME_N_ROWS; ri++) {
		if (sequence_symbol == game_copy->gameBoard[ri][ci]) {
			if (sequence_symbol != SP_FIAR_GAME_EMPTY_ENTRY) {
				if (sequence_len < 3) { /* it's not 4-in-a-column yet*/
					sequence_len += 1;
				} else { /* found 4-in-a-column! */
					if (sequence_symbol == SP_FIAR_GAME_PLAYER_1_SYMBOL) { /* player 1 won */
						return 1;
					} else { /* player 2 won */
						return 2;
					}
				}
			}
		} else { /* it's a new sequence */
			sequence_symbol = game_copy->gameBoard[ri][ci];
			sequence_len = 0;
		}
	}
	return 0;
}

/* return 0 if no 4-in-a-column in this diagonal
 * return 1 if there is 4 in a diagonal - for player 1
 * return 2 if there is 4 in a diagonal - for player 2
 */
int four_in_a_diagonal(SPFiarGame* game_copy, int ri, int ci) {
	int sequence_len = 0; /* the length of the current sequence */
	char sequence_symbol = game_copy->gameBoard[ri][ci]; /* no player sequence */
	while ((ri >= 0) && (ci < SP_FIAR_GAME_N_COLUMNS)) {
		if (sequence_symbol == game_copy->gameBoard[ri][ci]) {
			if (sequence_symbol != SP_FIAR_GAME_EMPTY_ENTRY) {
				if (sequence_len < 3) { /* it's not 4-in-a-column yet*/
					sequence_len += 1;
				} else { /* found 4-in-a-column! */
					if (sequence_symbol == SP_FIAR_GAME_PLAYER_1_SYMBOL) { /* player 1 won */
						return 1;
					} else { /* player 2 won */
						return 2;
					}
				}
			}
		} else { /* it's a new sequence */
			sequence_symbol = game_copy->gameBoard[ri][ci];
			sequence_len = 0;
		}
		/* moving in diagonal */
		ri -= 1;
		ci += 1;
	}
	return 0;
}

int is_board_full(SPFiarGame* game_copy) {
	for (int ci = 0; ci < SP_FIAR_GAME_N_COLUMNS; ci++) {
		if (game_copy->tops[ci] < SP_FIAR_GAME_N_ROWS) {
			return 0; /* the board isn't full - found empty slot */
		}
	}
	return 1; /* the board is full */
}

int diagonal_winning(SPFiarGame* src) {
	for (int ri = 0; ri < SP_FIAR_GAME_N_ROWS; ri++) {
		int diagonal_winner = four_in_a_diagonal(src, ri, 0);
		if (diagonal_winner != 0) {
			return diagonal_winner;
		}
	}
	for (int ci = 0; ci < SP_FIAR_GAME_N_COLUMNS; ci++) {
		int diagonal_winner = four_in_a_diagonal(src, SP_FIAR_GAME_N_ROWS - 1, ci);
		if (diagonal_winner != 0) {
			return diagonal_winner;
		}
	}
	return 0; /* no winner in any diagonal */
}

/**
* Checks if there's a winner in the specified game status. The function returns either
* SP_FIAR_GAME_PLAYER_1_SYMBOL or SP_FIAR_GAME_PLAYER_2_SYMBOL in case there's a winner, where
* the value returned is the symbol of the winner. If the game is over and there's a tie
* then the value SP_FIAR_GAME_TIE_SYMBOL is returned. in any other case the null characters
* is returned.
* @param src - the source game
* @return
* SP_FIAR_GAME_PLAYER_1_SYMBOL - if player 1 won
* SP_FIAR_GAME_PLAYER_2_SYMBOL - if player 2 won
* SP_FIAR_GAME_TIE_SYMBOL - If the game is over and there's a tie
* null character - otherwise
*/

char spFiarCheckWinner(SPFiarGame* src){
	int diagonal_winner = diagonal_winning(src);
	if (diagonal_winner != 0) {
		if (diagonal_winner == 1)
			return SP_FIAR_GAME_PLAYER_1_SYMBOL;
		else
			return SP_FIAR_GAME_PLAYER_2_SYMBOL;
	}
	for (int ri = 0; ri < SP_FIAR_GAME_N_ROWS; ri++) {
		int row_winner = four_in_a_row(src, ri);
		if (row_winner != 0) {
			if (row_winner == 1)
				return SP_FIAR_GAME_PLAYER_1_SYMBOL;
			else
				return SP_FIAR_GAME_PLAYER_2_SYMBOL;
		}
	}
	for (int ci = 0; ci < SP_FIAR_GAME_N_COLUMNS; ci++) {
		int column_winner = four_in_a_column(src, ci);
		if (column_winner != 0) {
			if (column_winner == 1)
				return SP_FIAR_GAME_PLAYER_1_SYMBOL;
			else
				return SP_FIAR_GAME_PLAYER_2_SYMBOL;
		}
	}

	if (is_board_full(src)) { /* checking if the game is full - and therefore a tie  */
		return SP_FIAR_GAME_TIE_SYMBOL;
	}

	return '\0';
}

/* return 0 if no 4-in-a-row in the sequence that starts at (ri, ci), in (delta_r, delta_c) direction
 * return 1 if there is 4 in-a-row - for player 1
 * return 2 if there is 4 in-a-row - for player 2
 */
char sequence_of_four(SPFiarGame* game_copy, int ri, int ci, int delta_r, int delta_c) {
	int sequence_len = 0; /* the length of the current sequence */
	char empty_symbol = ' '; /* no player sequence */
	char current_symbol = game_copy->gameBoard[ri][ci];
	if (current_symbol != empty_symbol) {
		sequence_len = 1;
	}
	while ((ri+delta_r >= 0) && (ri+delta_r < SP_FIAR_GAME_N_ROWS) && (ci+delta_c >= 0)
			&& (ci+delta_c < SP_FIAR_GAME_N_COLUMNS) && (sequence_len < 4)) {
		ri += delta_r;
		ci += delta_c;
		char next_symbol = game_copy->gameBoard[ri][ci];
		if (next_symbol == current_symbol) {
			if (current_symbol != empty_symbol) {
				sequence_len += 1;
			}
		} else if (next_symbol != empty_symbol){
			current_symbol = next_symbol;
		} else { /* next-symbol is the empty symbol */
			current_symbol = next_symbol;
			int sequence_len = 0;
		}
	}
	if (sequence_len == 4) {
		return current_symbol; /* the player that has won */
	} else {
		return empty_symbol;
	}
}

char spFiarCheckWinner2(SPFiarGame* src){
	char winning_seq_symbol;
	int somebody_won = 0;
	for (int ri = 0; ri < SP_FIAR_GAME_N_ROWS; ri++) {
		winning_seq_symbol = sequence_of_four(src, ri, 0, 0, 1); /* check the row */
		if (winning_seq_symbol != ' ') {
			return winning_seq_symbol;
		}
		winning_seq_symbol = sequence_of_four(src, ri, 0, 1, 1); /* check the diagonal - right-up*/
		if (winning_seq_symbol != ' ') {
			return winning_seq_symbol;
		}
		winning_seq_symbol = sequence_of_four(src, ri, 0, -1, 1); /* check the diagonal - right-down*/
		if (winning_seq_symbol != ' ') {
			return winning_seq_symbol;
		}
	}
	for (int ri = 0; ri < SP_FIAR_GAME_N_ROWS; ri++) {
		winning_seq_symbol = sequence_of_four(src, ri, SP_FIAR_GAME_N_COLUMNS-1, 1, 1); /* check the diagonal - left-up*/
		if (winning_seq_symbol != ' ') {
			return winning_seq_symbol;
		}
		winning_seq_symbol = sequence_of_four(src, ri, SP_FIAR_GAME_N_COLUMNS-1, -1, -1); /* check the diagonal - left-down*/
		if (winning_seq_symbol != ' ') {
			return winning_seq_symbol;
		}
	}
	for (int ci = 0; ci < SP_FIAR_GAME_N_COLUMNS; ci++) {
		winning_seq_symbol = sequence_of_four(src, 0, ci, 1, 0);
		if (winning_seq_symbol != ' ') {
			return winning_seq_symbol;
		}
	}
	if (is_board_full(src)) { /* checking if the game is full - and therefore a tie  */
		return SP_FIAR_GAME_TIE_SYMBOL;
	}

	return '\0';
}


