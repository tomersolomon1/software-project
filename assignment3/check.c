#include "SPMainAux.h"
#include "SPFIARGame.h"
#define HISTORY_SIZE 10

int main(){
	int moves[] = {3,4,3,3,5,4,2};
	int n = sizeof(moves)/sizeof(moves[0]);

	SPFiarGame* game = spFiarGameCreate(HISTORY_SIZE * 2);
	unsigned int difficulty_level = 6;
	int col_for_comp_move = -1;

	for(int i=0; i<n ; i++){
		//user move
		printf("user move:\n");
		fflush(stdout);
		spFiarGameSetMove(game, moves[i] - 1);
		spFiarGamePrintBoard(game);
		if(spFiarCheckWinner(game) != '\0'){
			printf("winner is:%c", spFiarCheckWinner(game));
			fflush(stdout);
			break;
		}
		//computer move
		printf("computer move:\n");
		fflush(stdout);
		col_for_comp_move = spMinimaxSuggestMove(game, difficulty_level);
		spFiarGameSetMove(game, col_for_comp_move);
		spFiarGamePrintBoard(game);
		if(spFiarCheckWinner(game) != '\0'){
			printf("winner is:%c", spFiarCheckWinner(game));
			fflush(stdout);
			break;
		}
	}
	return 0;
}
