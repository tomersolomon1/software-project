/*
 * SPMiniMax.c
 *
 *  Created on: May 31, 2017
 *      Author: sapir
 */

#include "SPMiniMax.h"

int spMinimaxSuggestMove2(SPFiarGame* currentGame,
		unsigned int maxDepth){
	for(int i=0; i < SP_FIAR_GAME_N_COLUMNS; i++){
		if(spFiarGameIsValidMove(currentGame, i)){
			return i;
		}
	}
	return 0;
}
