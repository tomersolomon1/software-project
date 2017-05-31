/*
 * SPMainAux.c
 *
 *  Created on: May 31, 2017
 *      Author: sapir
 */
#include "SPMainAux.h"

unsigned int get_difficulty_level(){
	printf("Please enter the difficulty level between [1-7]:\n");
	fflush(stdout);
	char input;
	int N = 0;
	int had_error = 0;
	while (((input = getchar()) != EOF) && (input != '\n')) {
		if(had_error) {continue;}
		else if('0' <= input && input <= '9'){
			N = N * 10;
			N += (input - '0');
		}
		else{
			had_error = 1;
		}
	}
	if (N < 1 || 7 < N || had_error){
		printf("Error: invalid level (should be between 1 to 7)\n");
		fflush(stdout);
		return 0;
	}
	return N;
}

void print_game_status(SPFiarGame* game){
	char winner = spFiarCheckWinner(game);
	if(winner == '\0'){
		printf("Please make the next move:\n");
		fflush(stdout);
	}
	else if(winner == SP_FIAR_GAME_PLAYER_1_SYMBOL){
		printf("Game over: you win\nPlease enter �quit� to exit or �restart� to start a new game!\n");
		fflush(stdout);
	}
	else if(winner == SP_FIAR_GAME_PLAYER_2_SYMBOL){
		printf("Game over: computer wins\nPlease enter �quit� to exit or �restart� to start a new game!\n");
		fflush(stdout);
	}
	else if(winner == SP_FIAR_GAME_TIE_SYMBOL){
		printf("Game over: it�s a tie\nPlease enter �quit� to exit or �restart� to start a new game!\n");
		fflush(stdout);
	}
}
SPCommand get_cmd(){
	char cmd_string[MAX_LENGTH];
	for(int i=0; i < MAX_LENGTH; i++){
		cmd_string[i] = '\0';
	}
	char input;
	int i = 0;
	while (((input = getchar()) != EOF) && (input != '\n')) {
		cmd_string[i] =  input;
		i++;
	}
	SPCommand cmd = spParserPraseLine(cmd_string);
	return cmd;
}

bool is_command_ok(SPCommand cmd, SPFiarGame* game){
	if(cmd.cmd == SP_INVALID_LINE){
		printf("Error: invalid command\n");
		fflush(stdout);
		return false;
	}
	else if(spFiarCheckWinner(game) != '\0'){
		if(cmd.cmd == SP_ADD_DISC || cmd.cmd == SP_SUGGEST_MOVE){
			printf("Error: the game is over\n");
			fflush(stdout);
			return false;
		}
	}
	else if(cmd.cmd == SP_ADD_DISC){
		if(cmd.arg < 1 || cmd.arg > SP_FIAR_GAME_N_COLUMNS || !cmd.validArg){
			printf("Error: column number must be in range 1-7\n");
			fflush(stdout);
			return false;
		}
		else if(!spFiarGameIsValidMove(game, cmd.arg - 1)){
			printf("Error: column %d is full\n", cmd.arg);
			fflush(stdout);
			return false;
		}
	}
	else if(cmd.cmd == SP_UNDO_MOVE){
		if(!spFiarGameIsUndoPossible(game)){
			printf("Error: cannot undo previous move!\n");
			fflush(stdout);
			return false;
		}
	}
	return true;
}

/*
 * making the actual command.
 * assuming the command is legal (previously need to run is_command_ok)
 * return false if need to restart the game, else true
 * */
bool make_command(SPCommand cmd, SPFiarGame* game, unsigned int depth){
	SP_FIAR_GAME_MESSAGE game_msg = SP_FIAR_GAME_INVALID_MOVE;
	if(cmd.cmd == SP_UNDO_MOVE){
		game_msg = spFiarGameUndoPrevMove(game);
		if(game_msg != SP_FIAR_GAME_SUCCESS){
			printf("Error: spFiarGameUndoPrevMove has failed");
			fflush(stdout);
			spFiarGameDestroy(game);
			exit(0);
		}
		game_msg = spFiarGameUndoPrevMove(game);
		if(game_msg != SP_FIAR_GAME_SUCCESS){
			printf("Error: spFiarGameUndoPrevMove has failed");
			fflush(stdout);
			spFiarGameDestroy(game);
			exit(0);
		}
		return true;
	}
	else if(cmd.cmd == SP_ADD_DISC){
		game_msg = spFiarGameSetMove(game, cmd.arg - 1);
		if(game_msg != SP_FIAR_GAME_SUCCESS){
			printf("Error: spFiarGameSetMove has failed");
			fflush(stdout);
			spFiarGameDestroy(game);
			exit(0);
		}
		if(spFiarCheckWinner(game) == '\0'){
			computer_move(game, depth);
		}
		return true;
	}
	else if(cmd.cmd == SP_QUIT){
		spFiarGameDestroy(game);
		exit(0);
	}
	else if(cmd.cmd == SP_RESTART){
		spFiarGameDestroy(game);
		return false;
	}
	else if(cmd.cmd == SP_SUGGEST_MOVE){
		int sugg_move = spMinimaxSuggestMove(game, depth);
		if(sugg_move < 0){
			printf("Error: spMinimaxSuggestMove has failed");
			fflush(stdout);
			spFiarGameDestroy(game);
			exit(0);
		}
		else{
			printf("Suggested move: drop a disc to column %d\n", sugg_move + 1);
			fflush(stdout);
		}
		return true;
	}
	return true;
}

void computer_move(SPFiarGame* game, unsigned int depth){
	int col_for_comp_move = spMinimaxSuggestMove(game, depth);
	if(col_for_comp_move < 0){
		printf("Error: spMinimaxSuggestMove has failed");
		fflush(stdout);
		spFiarGameDestroy(game);
		exit(0);
	}
	else{
		SP_FIAR_GAME_MESSAGE game_msg = spFiarGameSetMove(game, col_for_comp_move);
		if(game_msg != SP_FIAR_GAME_SUCCESS){
			printf("Error: spFiarGameSetMove has failed");
			fflush(stdout);
			spFiarGameDestroy(game);
			exit(0);
		}
	}
}



