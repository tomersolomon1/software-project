#include "SPFIARGame.h"
#include <stdlib.h>

SPFiarGame *create_board() {
	SPFiarGame* new_board = (SPFiarGame *) malloc(sizeof(SPFiarGame));
	for (int i = 0; i < 6; i++) {
		for(int j = 0; j < 7; j++) {
			new_board->gameBoard[i][j] = 2; /* defualt value, means empty */
		}
	}
	return new_board;
}

/* creating another copy of SPFiarGame */
SPFiarGame* copy_board(SPFiarGame* SPFiarGame) {
	SPFiarGame* new_SPFiarGame = (SPFiarGame *) malloc(sizeof(SPFiarGame));
	SPFiarGame newboard;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 6; j++) {
			new_SPFiarGame->gameBoard[i][j] = SPFiarGame->gameBoard[i][j];
		}
	}
	for (int i = 0; i < 6; i++) {
		new_SPFiarGame->tops[i] = SPFiarGame->tops[i];
	}
	new_SPFiarGame->currentPlayer = SPFiarGame->currentPlayer;

	return new_SPFiarGame;
}


