/*
 * SPMiniMax.c
 *
 *  Created on: May 31, 2017
 *      Author: sapir
 */

#include "SPMiniMax.h"
#include "SPFIARGame.h"
#include <limits.h>
#include <stddef.h>


/* we can assume the game is not over - there is at least one legal move, and the nobody has won yet */
int spMinimaxSuggestMove(SPFiarGame* currentGame, unsigned int maxDepth) {
	SPFiarGame* game_copy = spFiarGameCopy(currentGame);
	if (game_copy != NULL) {
		move_value best_move;
		spFiarGamePrintBoard(game_copy);
		best_move = minimaxAlgo(game_copy, maxDepth);
		spFiarGameDestroy(game_copy);
		if (best_move.move != -1) {
			return best_move.move;
		} else {
			/* no legal move - some problem occurred  */
			/* according to the return value we'll be able to who won / there was a tie */
			return -1;
		}
	} else { /* memory problem */
		return -2;
	}
}

