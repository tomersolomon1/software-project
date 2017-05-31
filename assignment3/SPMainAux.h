#ifndef SPMAINAUX_H_
#define SPMAINAUX_H_

#include "SPFIARGame.h"
#include "SPFIARParser.h"
#include "SPMiniMax.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LENGTH 1024
//put auxiliary functions and constants used by the main function here.

/*
 * getting the difficulty level from the user
 * returns the difiiculty level if its 0<=level<=7
 * otherwise returns 0
 * */
unsigned int get_difficulty_level();

/*
 * prints the game status: winner, tie, or continue playing
 * */
void print_game_status(SPFiarGame* game);

/*
 * getting the command ad input from the user
 * returns the command parsed
 * */
SPCommand get_cmd();

/*
 * checks if the command is valid
 * returns true if valid
 * otherwise, return false, and print the relevant error message
 * */
bool is_command_ok(SPCommand cmd, SPFiarGame* game);

/*
 * making the actual command.
 * assuming the command is legal (previously need to run is_command_ok)
 * return false if need to restart the game, else true
 * */
bool make_command(SPCommand cmd, SPFiarGame* game, unsigned int depth);

/**
 * does the computer move by suggest_move
 * */
void computer_move(SPFiarGame* game, unsigned int depth);

#endif
