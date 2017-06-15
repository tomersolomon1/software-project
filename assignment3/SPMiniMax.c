/*
 * SPMiniMax.c
 *
 *  Created on: May 31, 2017
 *      Author: sapir
 */

#include "SPMiniMax.h"
#include "SPFIARGame.h"
#include "SPMiniMaxNode.h"
#include <limits.h>
#include <stddef.h>

int spMinimaxSuggestMove(SPFiarGame* currentGame, unsigned int maxDepth) {
	if ((currentGame == NULL) || (maxDepth <= 0)) {
		return -1;
	}

	SPFiarGame* game_copy = spFiarGameCopy(currentGame);
	if (game_copy != NULL) {
		move_value best_move;
		best_move = minimaxAlgo(game_copy, maxDepth);
		spFiarGameDestroy(game_copy);
		if (best_move.move != -1) {
			return best_move.move;
		} else {
			/* no legal move - some problem occurred  */
			/* according to the return value we'll be able to who won / there was a tie */
			return -3;
		}
	} else { /* memory problem */
		return -2;
	}
}

