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

#include "SPFIARGame.h"

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
		for(int j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++){
			dst->gameBoard[i][j] = (src->gameBoard)[i][j];
		}
	}
	SPArrayList* history  = spArrayListCopy(src->history);
	if(history == NULL){
		return NULL;
	}
	spArrayListDestroy(dst->history);
	dst->history = history;
	return dst;
}

void spFiarGameDestroy(SPFiarGame* src){
	if(src != NULL){
		spArrayListDestroy(src->history);
		free(src);
	}
}

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

bool spFiarGameIsUndoPossible(SPFiarGame* src){
	if(spArrayListSize(src->history) == 0){
		return false;
	}
	return true;
}

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
	printf(" \n");
	fflush(stdout);
	return SP_FIAR_GAME_SUCCESS;
}

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

/*
 * checking if the board is full,
 * if its full return 1, else 0
 * */
int is_board_full(SPFiarGame* game_copy) {
	for (int ci = 0; ci < SP_FIAR_GAME_N_COLUMNS; ci++) {
		if (game_copy->tops[ci] < SP_FIAR_GAME_N_ROWS) {
			return 0; /* the board isn't full - found empty slot */
		}
	}
	return 1; /* the board is full */
}

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
			sequence_len = 1; /* we are starting a new sequence */
		} else { /* next-symbol is the empty symbol */
			current_symbol = next_symbol;
			sequence_len = 0;
		}
	}
	if (sequence_len == 4) {
		return current_symbol; /* the player that has won  */
	} else {
		return empty_symbol;
	}
}

char spFiarCheckWinner(SPFiarGame* src){
	char winning_seq_symbol;
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
	if (is_board_full(src)) { /* checking if the game is full - and therefore a tie   */
		return SP_FIAR_GAME_TIE_SYMBOL;
	}

	return '\0';
}
