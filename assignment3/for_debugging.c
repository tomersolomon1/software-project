/*
 * for_debugging.c
 *
 *  Created on: May 31, 2017
 *      Author: sapir
 */
#include "SPMainAux.h"
#include "SPFIARGame.h"
#define HISTORY_SIZE 10

int main(){
	unsigned int depth = 3;
	SPFiarGame* game = spFiarGameCreate(HISTORY_SIZE * 2);

	spFiarGameSetMove(game, 1);
	spFiarGamePrintBoard(game);

	int col_for_comp_move = spMinimaxSuggestMove(game, depth);
	spFiarGameSetMove(game, col_for_comp_move);
	spFiarGamePrintBoard(game);

	spFiarGameSetMove(game, 4);
	spFiarGamePrintBoard(game);

	col_for_comp_move = spMinimaxSuggestMove(game, depth);
	spFiarGameSetMove(game, col_for_comp_move);
	spFiarGamePrintBoard(game);
}
