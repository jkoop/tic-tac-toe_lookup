#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define CLEAR 0
#define WHITE 1
#define BLACK 2
#define CONT 3
#define DRAW 4

typedef struct {
	uint_fast8_t spaces[9];
	uint_fast8_t end_condition;
	uint_fast8_t turn_offset;
} board;

// who won?
uint_fast8_t get_end_condition(const board *board) {
	// check for win
	if (board->spaces[0] != CLEAR && board->spaces[0] == board->spaces[1] &&
	    board->spaces[1] == board->spaces[2])
		return board->spaces[0];
	if (board->spaces[3] != CLEAR && board->spaces[3] == board->spaces[4] &&
	    board->spaces[4] == board->spaces[5])
		return board->spaces[3];
	if (board->spaces[6] != CLEAR && board->spaces[6] == board->spaces[7] &&
	    board->spaces[7] == board->spaces[8])
		return board->spaces[6];
	if (board->spaces[0] != CLEAR && board->spaces[0] == board->spaces[3] &&
	    board->spaces[3] == board->spaces[6])
		return board->spaces[0];
	if (board->spaces[1] != CLEAR && board->spaces[1] == board->spaces[4] &&
	    board->spaces[4] == board->spaces[7])
		return board->spaces[1];
	if (board->spaces[2] != CLEAR && board->spaces[2] == board->spaces[5] &&
	    board->spaces[5] == board->spaces[8])
		return board->spaces[2];
	if (board->spaces[0] != CLEAR && board->spaces[0] == board->spaces[4] &&
	    board->spaces[4] == board->spaces[8])
		return board->spaces[0];
	if (board->spaces[6] != CLEAR && board->spaces[6] == board->spaces[4] &&
	    board->spaces[4] == board->spaces[2])
		return board->spaces[6];

	// check for draw
	if ((board->spaces[0] != CLEAR && board->spaces[1] != CLEAR && board->spaces[2] != CLEAR) &&
	    (board->spaces[3] != CLEAR && board->spaces[4] != CLEAR && board->spaces[5] != CLEAR) &&
	    (board->spaces[6] != CLEAR && board->spaces[7] != CLEAR && board->spaces[8] != CLEAR))
		return DRAW;

	return CONT;
}

board boards[9];
const board empty_board = {CLEAR, CLEAR, CLEAR, CLEAR, CLEAR, CLEAR, CLEAR, CLEAR, CLEAR, CONT, 0};
uint_fast8_t depth_now = 1;
uint_fast8_t turn_now, turn_offset_now, skip_turn;
uint_fast8_t reset_turn_offset = 0;

int main() {
	// printf("depth\tturn\tparent board\tturnoff\tcond\tchild board\tturnoff\tcond\tdebug\n");
	memcpy(&boards[0], &empty_board, sizeof empty_board);

	while (1 > 0) {
		turn_now = (depth_now - 1) % 2 + WHITE;
		memcpy(&boards[depth_now].spaces, &boards[depth_now - 1].spaces, sizeof boards[depth_now].spaces);
		if (reset_turn_offset) {
			boards[depth_now].turn_offset = 0;
			reset_turn_offset = 0;
		}
		turn_offset_now = 0;
		for (uint_fast8_t i = 0; i < 9; i++)
			if (boards[depth_now].spaces[i] == CLEAR) {
				if (turn_offset_now == boards[depth_now].turn_offset) {
					boards[depth_now].spaces[i] = turn_now;
					skip_turn = 0;
					goto did_a_turn;
				}
				turn_offset_now++;
			}
		skip_turn = 1;
	did_a_turn:
		boards[depth_now].end_condition = get_end_condition(&boards[depth_now]);

		// print relationship -->>
		if (skip_turn == 0) {
			for (uint_fast8_t i = 0; i < 9; i++)
				printf("%i", boards[depth_now - 1].spaces[i]);
			printf("\t");

			if (turn_now == WHITE) printf("WHITE\t");
			if (turn_now == BLACK) printf("BLACK\t");

			for (uint_fast8_t i = 0; i < 9; i++)
				printf("%i", boards[depth_now].spaces[i]);
			printf("\t");

			if (boards[depth_now].end_condition == CONT) printf("CONT");
			if (boards[depth_now].end_condition == WHITE) printf("WHITE");
			if (boards[depth_now].end_condition == BLACK) printf("BLACK");
			if (boards[depth_now].end_condition == DRAW) printf("DRAW");
			// <<-- print relationship

			// if (skip_turn == 1) printf("\tskip");
			printf("\n");
		}

		if (skip_turn == 1 || boards[depth_now].end_condition == DRAW) {
			depth_now--;
			boards[depth_now].turn_offset++;
			if (depth_now == 0) return 0;
		} else if (boards[depth_now].end_condition == CONT) {
			depth_now++;
			reset_turn_offset = 1;
		} else {
			boards[depth_now].turn_offset++;
		}
	}
}
