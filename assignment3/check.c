#include "SPMainAux.h"
#include "SPFIARGame.h"
#include "SPMiniMaxNode.h"
#define HISTORY_SIZE 10

int main2222(){
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
		if (i == n-1) {
			difficulty_level = 6;
		}
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

void check_eval1() {
	SPFiarGame* game = spFiarGameCreate(10);
	char symbol1 = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	char symbol2 = SP_FIAR_GAME_PLAYER_2_SYMBOL;
	game->gameBoard[0][1] = symbol1;
	game->gameBoard[1][1] = symbol1;
	game->gameBoard[2][1] = symbol2;
	game->gameBoard[3][1] = symbol1;
	game->gameBoard[4][1] = symbol1;

	game->gameBoard[0][2] = symbol1;
	game->gameBoard[1][2] = symbol2;
	game->gameBoard[2][2] = symbol2;

	game->gameBoard[0][3] = symbol2;
	game->gameBoard[1][3] = symbol2;
	game->gameBoard[2][3] = symbol1;

	game->gameBoard[0][4] = symbol1;

	game->gameBoard[0][5] = symbol2;

	game->gameBoard[0][6] = symbol2;


	int evaluation = evaluate_board(game);
	spFiarGamePrintBoard(game);
	printf("value of the board: %d", evaluation);
	spFiarGameDestroy(game);

}

void check_eval2() {  /* should be victory for player 1 */
	SPFiarGame* game = spFiarGameCreate(10);
	char symbol1 = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	char symbol2 = SP_FIAR_GAME_PLAYER_2_SYMBOL;

	game->gameBoard[0][2] = symbol1;
	game->gameBoard[1][2] = symbol1;
	game->gameBoard[2][2] = symbol1;

	game->gameBoard[0][3] = symbol2;
	game->gameBoard[1][3] = symbol2;
	game->gameBoard[2][3] = symbol2;

	game->gameBoard[0][4] = symbol1;
	game->gameBoard[1][4] = symbol1;
	game->gameBoard[2][4] = symbol1;

	int evaluation = evaluate_board(game);
	spFiarGamePrintBoard(game);
	printf("value of the board: %d", evaluation);
	spFiarGameDestroy(game);
}

void check_eval3() {  /* should be victory for player 1 */
	SPFiarGame* game = spFiarGameCreate(10);
	char symbol1 = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	//char symbol2 = SP_FIAR_GAME_PLAYER_2_SYMBOL;

	game->gameBoard[0][6] = symbol1;
	game->gameBoard[1][6] = symbol1;
	game->gameBoard[2][6] = symbol1;


	int evaluation = evaluate_board(game);
	spFiarGamePrintBoard(game);
	printf("value of the board: %d", evaluation);
	spFiarGameDestroy(game);
}

void check_eval_with_uri1() {
	SPFiarGame* game = spFiarGameCreate(10);
	char symbol1 = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	char symbol2 = SP_FIAR_GAME_PLAYER_2_SYMBOL;

	/* first row */
	game->gameBoard[0][0] = symbol1;
	game->gameBoard[0][1] = symbol1;
	game->gameBoard[0][2] = symbol1;
	game->gameBoard[0][3] = symbol2;
	game->gameBoard[0][4] = symbol2;
	game->gameBoard[0][6] = symbol2;

	/* second row */
	game->gameBoard[1][1] = symbol2;
	game->gameBoard[1][2] = symbol2;
	game->gameBoard[1][3] = symbol1;
	game->gameBoard[1][4] = symbol1;

	/* thrid row */
	game->gameBoard[2][2] = symbol2;
	game->gameBoard[2][3] = symbol2;
	game->gameBoard[2][4] = symbol1;

	/* forth row */
	game->gameBoard[3][2] = symbol1;
	game->gameBoard[3][3] = symbol1;
	game->gameBoard[3][4] = symbol2;

	int evaluation = evaluate_board(game);
	spFiarGamePrintBoard(game);
	printf("value of the board: %d\n", evaluation);
	fflush(stdout);
	spFiarGameDestroy(game);
}

void check_eval_with_uri2() {
	SPFiarGame* game = spFiarGameCreate(10);
	char symbol1 = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	char symbol2 = SP_FIAR_GAME_PLAYER_2_SYMBOL;

	/* first row */
	game->gameBoard[0][0] = symbol1;
	game->gameBoard[0][1] = symbol2;
	game->gameBoard[0][2] = symbol2;
	game->gameBoard[0][3] = symbol1;
	game->gameBoard[0][4] = symbol2;
	game->gameBoard[0][5] = symbol2;
	game->gameBoard[0][6] = symbol2;

	/* second row */
	game->gameBoard[1][0] = symbol2;
	game->gameBoard[1][1] = symbol1;
	game->gameBoard[1][2] = symbol2;
	game->gameBoard[1][3] = symbol1;
	game->gameBoard[1][4] = symbol1;
	game->gameBoard[1][5] = symbol2;

	/* thrid row */
	game->gameBoard[2][0] = symbol1;
	game->gameBoard[2][1] = symbol1;
	game->gameBoard[2][2] = symbol2;
	game->gameBoard[2][3] = symbol1;
	game->gameBoard[2][4] = symbol1;

	/* forth row */
	game->gameBoard[3][0] = symbol2;
	game->gameBoard[3][1] = symbol2;
	game->gameBoard[3][2] = symbol1;
	game->gameBoard[3][3] = symbol2;

	/* fifth row */
	game->gameBoard[4][0] = symbol1;
	game->gameBoard[4][1] = symbol2;
	game->gameBoard[4][2] = symbol2;

	/* sixth row */
	game->gameBoard[5][0] = symbol2;
	game->gameBoard[5][1] = symbol2;

	int evaluation = evaluate_board(game);
	spFiarGamePrintBoard(game);
	printf("value of the board: %d\n", evaluation);
	fflush(stdout);
	spFiarGameDestroy(game);
}

void check_eval_with_uri3() {
	SPFiarGame* game = spFiarGameCreate(10);
	char symbol1 = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	char symbol2 = SP_FIAR_GAME_PLAYER_2_SYMBOL;

	/* first row */
	game->gameBoard[0][0] = symbol2;
	game->gameBoard[0][1] = symbol2;
	game->gameBoard[0][2] = symbol2;
	game->gameBoard[0][3] = symbol1;
	game->gameBoard[0][4] = symbol2;
	game->gameBoard[0][5] = symbol1;
	game->gameBoard[0][6] = symbol1;

	/* second row */
	game->gameBoard[1][0] = symbol2;
	game->gameBoard[1][1] = symbol2;
	game->gameBoard[1][2] = symbol2;
	game->gameBoard[1][3] = symbol1;
	game->gameBoard[1][4] = symbol2;
	game->gameBoard[1][5] = symbol1;
	game->gameBoard[1][6] = symbol2;

	/* thrid row */
	game->gameBoard[2][2] = symbol1;
	game->gameBoard[2][3] = symbol2;
	game->gameBoard[2][4] = symbol1;

	/* forth row */
	game->gameBoard[3][2] = symbol1;
	game->gameBoard[3][3] = symbol2;


	int evaluation = evaluate_board(game);
	spFiarGamePrintBoard(game);
	printf("value of the board: %d\n", evaluation);
	fflush(stdout);
	spFiarGameDestroy(game);
}

void check_eval_with_uri4() {
	SPFiarGame* game = spFiarGameCreate(10);
	char symbol1 = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	char symbol2 = SP_FIAR_GAME_PLAYER_2_SYMBOL;

	/* first row */
	game->gameBoard[0][0] = symbol2;

	game->gameBoard[0][6] = symbol1;

	/* second row */
	game->gameBoard[1][0] = symbol2;
	game->gameBoard[1][1] = symbol2;
	game->gameBoard[1][4] = symbol2;
	game->gameBoard[1][6] = symbol2;

	/* thrid row */
	game->gameBoard[2][2] = symbol1;
	game->gameBoard[2][3] = symbol2;
	game->gameBoard[2][4] = symbol1;

	/* forth row */
	game->gameBoard[3][2] = symbol1;
	game->gameBoard[3][3] = symbol2;

	/* corners */
	game->gameBoard[5][0] = symbol1;
	game->gameBoard[5][6] = symbol1;



	int evaluation = evaluate_board(game);
	spFiarGamePrintBoard(game);
	printf("value of the board: %d\n", evaluation);
	fflush(stdout);
	spFiarGameDestroy(game);
}

void check_eval_with_uri5() {
	SPFiarGame* game = spFiarGameCreate(10);
	char symbol1 = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	char symbol2 = SP_FIAR_GAME_PLAYER_2_SYMBOL;

	/* second row */
	game->gameBoard[1][1] = symbol2;
	game->gameBoard[1][4] = symbol1;
	game->gameBoard[1][6] = symbol2;

	/* forth row */
	game->gameBoard[3][2] = symbol1;

	/* corners */
	game->gameBoard[5][0] = symbol1;
	game->gameBoard[5][6] = symbol1;



	int evaluation = evaluate_board(game);
	spFiarGamePrintBoard(game);
	printf("value of the board: %d\n", evaluation);
	fflush(stdout);
	spFiarGameDestroy(game);
}

void check_eval_with_uri6() {
	SPFiarGame* game = spFiarGameCreate(10);
	char symbol1 = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	//char symbol2 = SP_FIAR_GAME_PLAYER_2_SYMBOL;

	/* corners */
	game->gameBoard[5][0] = symbol1;
	game->gameBoard[5][6] = symbol1;



	int evaluation = evaluate_board(game);
	spFiarGamePrintBoard(game);
	printf("value of the board: %d\n", evaluation);
	fflush(stdout);
	spFiarGameDestroy(game);
}

void tie_board() {

	SPFiarGame* game = spFiarGameCreate(10);
	char symbol1 = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	char symbol2 = SP_FIAR_GAME_PLAYER_2_SYMBOL;
	char game_status;

	game->tops[0] = 6;
	game->tops[1] = 6;
	game->tops[2] = 6;
	game->tops[3] = 6;
	game->tops[4] = 6;
	game->tops[5] = 6;
	game->tops[6] = 6;

	/* first row */
	game->gameBoard[0][0] = symbol2;
	game->gameBoard[0][1] = symbol1;
	game->gameBoard[0][2] = symbol2;
	game->gameBoard[0][3] = symbol1;
	game->gameBoard[0][4] = symbol2;
	game->gameBoard[0][5] = symbol1;
	game->gameBoard[0][6] = symbol2;

	/* second row */
	game->gameBoard[1][0] = symbol1;
	game->gameBoard[1][1] = symbol2;
	game->gameBoard[1][2] = symbol1;
	game->gameBoard[1][3] = symbol2;
	game->gameBoard[1][4] = symbol1;
	game->gameBoard[1][5] = symbol2;
	game->gameBoard[1][6] = symbol1;

	/* third row */
	game->gameBoard[2][0] = symbol1;
	game->gameBoard[2][1] = symbol2;
	game->gameBoard[2][2] = symbol1;
	game->gameBoard[2][3] = symbol2;
	game->gameBoard[2][4] = symbol1;
	game->gameBoard[2][5] = symbol2;
	game->gameBoard[2][6] = symbol1;

	/* forth row */
	game->gameBoard[3][0] = symbol2;
	game->gameBoard[3][1] = symbol1;
	game->gameBoard[3][2] = symbol2;
	game->gameBoard[3][3] = symbol1;
	game->gameBoard[3][4] = symbol2;
	game->gameBoard[3][5] = symbol1;
	game->gameBoard[3][6] = symbol2;

	/* fifth row */
	game->gameBoard[4][0] = symbol1;
	game->gameBoard[4][1] = symbol2;
	game->gameBoard[4][2] = symbol1;
	game->gameBoard[4][3] = symbol2;
	game->gameBoard[4][4] = symbol1;
	game->gameBoard[4][5] = symbol2;
	game->gameBoard[4][6] = symbol1;

	/* fifth row */
	game->gameBoard[5][0] = symbol1;
	game->gameBoard[5][1] = symbol2;
	game->gameBoard[5][2] = symbol1;
	game->gameBoard[5][3] = symbol2;
	game->gameBoard[5][4] = symbol1;
	game->gameBoard[5][5] = symbol2;
	game->gameBoard[5][6] = symbol1;

	int evaluation = evaluate_board(game);
	spFiarGamePrintBoard(game);
	printf("value of the board: %d\n", evaluation);
	fflush(stdout);
	game_status = spFiarCheckWinner(game);
	printf("game-status: %c\n", game_status);

	spFiarGameDestroy(game);
}

void losing_boards() {
	move_value fake_move = {0, 0};
	SPFiarGame* game = spFiarGameCreate(10);
	char symbol1 = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	char symbol2 = SP_FIAR_GAME_PLAYER_2_SYMBOL;
	char game_status;
	/* winning in a row */
	game->gameBoard[5][0] = symbol1;
	game->gameBoard[5][1] = symbol1;
	game->gameBoard[5][2] = symbol1;
	game->gameBoard[5][3] = symbol1;

	int evaluation = evaluate_board(game);

	game_status = spFiarCheckWinner(game);
	is_the_game_over(game, &fake_move);
	printf("winner: %c, value of the board: %d, fake-move.value: %d\n", game_status, evaluation, fake_move.value);
	fake_move.value = 0;

	fflush(stdout);
	spFiarGameDestroy(game);

	/* winning in a column */
	game = spFiarGameCreate(10);
	game->gameBoard[1][0] = symbol1;
	game->gameBoard[2][0] = symbol1;
	game->gameBoard[3][0] = symbol1;
	game->gameBoard[4][0] = symbol1;

	evaluation = evaluate_board(game);
	spFiarGamePrintBoard(game);
	game_status = spFiarCheckWinner(game);
	is_the_game_over(game, &fake_move);
	printf("winner: %c, value of the board: %d, fake-move.value: %d\n", game_status, evaluation, fake_move.value);
	fake_move.value = 0;
	fflush(stdout);
	spFiarGameDestroy(game);

	/* winning in a diagonal 1 */
	game = spFiarGameCreate(10);
	game->gameBoard[5][0] = symbol1;
	game->gameBoard[4][1] = symbol1;
	game->gameBoard[3][2] = symbol1;
	game->gameBoard[2][3] = symbol1;

	evaluation = evaluate_board(game);
	spFiarGamePrintBoard(game);
	game_status = spFiarCheckWinner(game);
	is_the_game_over(game, &fake_move);
	printf("winner: %c, value of the board: %d, fake-move.value: %d\n", game_status, evaluation, fake_move.value);
	fake_move.value = 0;
	fflush(stdout);
	spFiarGameDestroy(game);

	/* winning in a diagonal 2 */
	game = spFiarGameCreate(10);
	game->gameBoard[3][0] = symbol1;
	game->gameBoard[2][1] = symbol1;
	game->gameBoard[1][2] = symbol1;
	game->gameBoard[0][3] = symbol1;

	evaluation = evaluate_board(game);
	spFiarGamePrintBoard(game);
	game_status = spFiarCheckWinner(game);
	is_the_game_over(game, &fake_move);
	printf("winner: %c, value of the board: %d, fake-move.value: %d\n", game_status, evaluation, fake_move.value);
	fake_move.value = 0;
	fflush(stdout);
	spFiarGameDestroy(game);

	/* winning in a diagonal 3 */
	game = spFiarGameCreate(10);
	game->gameBoard[3][2] = symbol2;
	game->gameBoard[2][3] = symbol2;
	game->gameBoard[1][4] = symbol2;
	game->gameBoard[0][5] = symbol2;

	evaluation = evaluate_board(game);
	spFiarGamePrintBoard(game);
	game_status = spFiarCheckWinner(game);
	is_the_game_over(game, &fake_move);
	printf("winner: %c, value of the board: %d, fake-move.value: %d\n", game_status, evaluation, fake_move.value);
	fake_move.value = 0;
	fflush(stdout);

	/* not a winnig board */

	game = spFiarGameCreate(10);
	game->gameBoard[3][2] = symbol2;
	game->gameBoard[2][3] = symbol2;
	game->gameBoard[1][4] = symbol2;
	evaluation = evaluate_board(game);
	spFiarGamePrintBoard(game);
	game_status = spFiarCheckWinner(game);
	is_the_game_over(game, &fake_move);
	printf("winner: %c, value of the board: %d, fake-move.value: %d\n", game_status, evaluation, fake_move.value);
	fake_move.value = 0;
	spFiarGameDestroy(game);
	fflush(stdout);
}

void check_scores() {
	check_eval1();
	printf("\n");
	fflush(stdout);
	check_eval3();
	printf("\n");
	fflush(stdout);

	check_eval_with_uri1();
	printf("\n");
	fflush(stdout);

	check_eval_with_uri2();
	printf("\n");
	fflush(stdout);

	check_eval_with_uri3();
	printf("\n");
	fflush(stdout);

	check_eval_with_uri4();
	printf("\n");
	fflush(stdout);

	check_eval_with_uri5();
	printf("\n");
	fflush(stdout);

	check_eval_with_uri6();

}

void check_eval() {
	losing_boards();
	tie_board();
	fflush(stdout);
}



