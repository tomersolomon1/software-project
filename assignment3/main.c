/*
 * main.c
 *
 *  Created on: May 31, 2017
 *      Author: sapir
 */
#include "SPMainAux.h"
#include "SPFIARGame.h"
#define HISTORY_SIZE 10

int main() {
	check_eval();
}
int main100(){
	while(1){
		unsigned int difficulty_level = get_difficulty_level();
		while (difficulty_level  == 0){
			difficulty_level = get_difficulty_level();
		}
		SPFiarGame* game = spFiarGameCreate(HISTORY_SIZE * 2);
		if(game == NULL){
			printf("Error: spFiarGameCreate has failed");
			fflush(stdout);
			exit(0);
		}
		SPCommand cmd;
		cmd.cmd = SP_INVALID_LINE;
		while(1){
			if(cmd.cmd != SP_SUGGEST_MOVE){
				spFiarGamePrintBoard(game);
			}
			print_game_status(game);
			cmd = get_cmd();
			while(!is_command_ok(cmd, game)){
				print_game_status(game);
				cmd = get_cmd();
			}
			if(!make_command(cmd, game, difficulty_level)){
				break;
			}
		}
	}

	return 0;
}

