/*
 * SPFIARParser.c
 *
 *  Created on: May 30, 2017
 *      Author: sapir
 */


#include "SPFIARParser.h"

//specify the maximum line length


/**
 * Checks if a specified string represents a valid integer. It is recommended
 * to use this function prior to calling the standard library function atoi.
 *
 * @return
 * true if the string represents a valid integer, and false otherwise.
 */
bool spParserIsInt(const char* str){
	int i = atoi(str);
	if(i != 0){
		return true;
	}
	for(int i = 0; i < strlen(str); i++){
		if(str[i] != '0'){
			return false;
		}
	}
	return true;
}

int get_place_1st_nonspace_char(const char* str){
	int i = 0;
	char c = str[i];
	while(c != '\n' && c != '\0' &&  (c == '\t' || c == ' ')){
		i++;
		c = str[i];
	}
	return i;
}

bool is_str1_begins_with_str2(const char* str1, const char* str2){
	if(strlen(str1) < strlen(str2)){
		return false;
	}
	int a = strncmp(str1, str2, strlen(str2));
	if(a == 0){
		return true;
	}
	return false;
}

/**
 * Parses a specified line. If the line is a command which has an integer
 * argument then the argument is parsed and is saved in the field arg and the
 * field validArg is set to true. In any other case then 'validArg' is set to
 * false and the value 'arg' is undefined
 *
 * @return
 * A parsed line such that:
 *   cmd - contains the command type, if the line is invalid then this field is
 *         set to INVALID_LINE
 *   validArg - is set to true if the command is add_disc and the integer argument
 *              is valid
 *   arg      - the integer argument in case validArg is set to true
 */
SPCommand spParserPraseLine(const char* str){
	SPCommand command;
	command.arg = 0;
	command.validArg = false;
	command.cmd = SP_INVALID_LINE;

	const char* commands[5];
	commands[0] = "suggest_move";
	commands[1] = "undo_move";
	commands[2] = "add_disc";
	commands[3] = "quit";
	commands[4] = "restart_game";

	SP_COMMAND comms[5];
	comms[0] = SP_SUGGEST_MOVE;
	comms[1] = SP_UNDO_MOVE;
	comms[2] = SP_ADD_DISC;
	comms[3] = SP_QUIT;
	comms[4] = SP_RESTART;

	int curr_pointer = get_place_1st_nonspace_char(str);
	int curr_cmd = 0;
	switch(str[curr_pointer]){ // now curr_pointer points the first nonspace char
		case 's':
			curr_cmd = 0;
			break;
		case 'u':
			curr_cmd = 1;
			break;
		case 'a':
			curr_cmd = 2;
			break;
		case 'q':
			curr_cmd = 3;
			break;
		case 'r':
			curr_cmd = 4;
			break;
		default:
			return command;
	}
	bool is_begin_ok = is_str1_begins_with_str2(str + curr_pointer, commands[curr_cmd]);
	if(!is_begin_ok){
		return command;
	}
	curr_pointer = curr_pointer + strlen(commands[curr_cmd]);
	const char *copy = str + curr_pointer;
	curr_pointer += get_place_1st_nonspace_char(copy); // now curr_pointer points the second nonspace char

	if(curr_cmd != 2){ 						//no more parameters
		if(str[curr_pointer] == '\0' || str[curr_pointer] == '\n'){
			command.cmd = comms[curr_cmd];
			command.validArg = true;
		}
		return command;
	}
	else if(curr_cmd == 2){ 				//need to get coloumn number
		command.cmd = comms[curr_cmd];
		bool had_number = false;
		int col = 0;
		while(str[curr_pointer] != '\0' && str[curr_pointer] != '\n'){
			if('0' <= str[curr_pointer] && str[curr_pointer] <= '9'){
				had_number = true;
				col = col * 10;
				col += (str[curr_pointer] - '0');
				curr_pointer++;
			}
			else{
				return command;
			}
		}
		if(!had_number){
			return command;
		}
		command.arg = col;
		command.validArg = true;
		return command;
	}
	return command;

}


