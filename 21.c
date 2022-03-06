// Advent of Code 2021 - Day 21
// Written by Henry Peaurt

// Why the hell doesn't it work. Literal trash. I'm never programming again.
// Note from later: I half-copied another solution and that's it, whatever.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "strmanip.h"
#include "memmanage.h"

// Types
typedef struct {
	int pos_1;
	int score_1;
	int pos_2;
	int score_2;
} game;

typedef struct {
	long wins_1;
	long wins_2;
	bool was_;
} result;

// Function declarations
game input_positions(void);
int run_practice(game gm);
game move_player(game gm_prev, int amount);
result run_quantum(game gm);

int
main(void)
{
	game gm = input_positions();

	int part_1 = run_practice(gm);
	result quantum = run_quantum(gm);
	long part_2 = quantum.wins_1 > quantum.wins_2 ? quantum.wins_1 : quantum.wins_2;

	printf("Part 1: %u\n", part_1);
	printf("Part 2: %lu\n", part_2);

	return 0;
}

game
input_positions(void)
{
	game gm = {
		.pos_1 = atoi(str_word(4, str_input())) - 1,
		.score_1 = 0,
		.pos_2 = atoi(str_word(4, str_input())) - 1,
		.score_2 = 0
	};

	return gm;
}

int
run_practice(game gm)
{
	int dice_num = 1;
	int throw_cnt = 0;

	while (gm.score_2 < 1000) {
		int throw_rslt = 3 * (dice_num + dice_num + 2) / 2;

		dice_num += 3;
		throw_cnt += 3;

		gm = move_player(gm, throw_rslt);
	}

	return throw_cnt * (gm.score_1 < gm.score_2 ? gm.score_1 : gm.score_2);
}

void add_res(result* res, result res1, int count);
result
run_quantum(game gm)
{
	static result state[10][21][10][21];
	result* curr = &(state[gm.pos_1][gm.score_1][gm.pos_2][gm.score_2]);
	result res = { 0 };

	if (curr->was_) {
		return *curr;
	} else if (gm.score_2 >= 21) {
		res.wins_2 = 1;
		return res;
	} else {
		add_res(&res, run_quantum(move_player(gm, 3)), 1);
		add_res(&res, run_quantum(move_player(gm, 4)), 3);
		add_res(&res, run_quantum(move_player(gm, 5)), 6);
		add_res(&res, run_quantum(move_player(gm, 6)), 7);
		add_res(&res, run_quantum(move_player(gm, 7)), 6);
		add_res(&res, run_quantum(move_player(gm, 8)), 3);
		add_res(&res, run_quantum(move_player(gm, 9)), 1);

		res.was_ = true;
		return res;
	}
}

void
add_res(result* res, result res1, int count)
{
	res->wins_1 += res1.wins_2 * count;
	res->wins_2 += res1.wins_1 * count;
}

game
move_player(game gm_prev, int amount)
{
	game gm_next;

	gm_next.pos_1 = gm_prev.pos_2;
	gm_next.score_1 = gm_prev.score_2;
	gm_next.pos_2 = (gm_prev.pos_1 + amount) % 10;
	gm_next.score_2 = gm_prev.score_1 + gm_next.pos_2 + 1;

	return gm_next;
}

/*
long
iter_universes(long universe[10][21][10][21], bool from_beginning)
{
	static int i;
	static int j;
	static int k;
	static int l;

	if (!from_beginning) {
		goto next;
	}

	for (i = 0; i < 10; i++) {
		for (j = 0; j < 21; j++) {
			for (k = 0; k < 10; k++) {
				for (l = 0; l < 21; l++) {
					return universe[i][j][k][l];
next:					continue;
				}
			}
		}
	}
}

bool
exist_games(long universe[10][21][10][21])
{
	for (int i = 0; i < 10 * 21 * 10 * 21; i++) {
		exists = iter_universes(universe) > 0;
		if (exists) {
			return true;
		}
	}

	return false;
}

void
run_multiverse(game* gm)
{
	int turn = 2;
	long player_1_wins = 0;
	long player_2_wins = 0;

	while (exist_games(universe)) {
		turn = (turn == 1) + 1;

		for (int i = 0; i < 10 * 21 * 10 * 21; i++) {
			long universes = iter_universes(universe);

			if (universes == 0) {
				continue;
			}

			for (int roll = 3; roll < 10; roll++) {
				int next_position = calc_position(
					turn == 1 ? universe[i].player_1.position : universe[i].player_2.position,
					roll
				);
				int factor = 1;

				if (roll == 4 || roll == 8) { factor = 3; }
				else if (roll == 5 || roll == 7) { factor = 6; }
				else if (roll == 6) { factor = 7; }

				if (turn == 1 && universe[i].player_1.score + next_position >= 21) {
					player_1_wins += universe[i].existing * factor;
					continue;
				} else if (tracker == 2 && universe[i].player_2.score + next_position >= 21) {
					player_2_wins += universe[i].existing * factor;
					continue;
				}

				game new_uni = universe[i];

				if (turn == 1) {
					new_uni.player_1.position = next_position;
					new_uni.player_1.score += next_position;
				} else { // turn == 2
					new_uni.player_2.position = next_position;
					new_uni.player_2.score += next_position;
				}

				new_uni.existing *= factor;
			}

			universe[i].existing = 0;
		}
	}
}
*/

/*
int
hash(game* gm)
{
	int hash = 0;

	hash = (hash + gm->player_1->position) * 100;
	hash = (hash + gm->player_1->score) * 100;
	hash = (hash + gm->player_2->position) * 100;
	hash = (hash + gm->player_2->score) * 100;

	return hash;
}
*/

/*
typedef enum {
	A,
	B
} playernum;

void
run_multiverse(game* gm)
{
	long game_state[10][21][10][21] = { 0 };
	long copy_state[10][21][10][21] = { 0 };
	long won_0 = 0;
	long won_1 = 0;
	int active_games = 1;
	playernum current = A;

	game_state[gm->player_1.position][0][gm->player_2.position][0] = 1;

	while (active_games > 0) {
		for (int spc0 = 0; spc0 < 10; spc0++) {
			for (int scr0 = 0; scr0 < 21; scr0++) {
				for (int spc1 = 0; spc1 < 10; spc1++) {
					for (int scr1 = 0; scr1 < 21; scr1++) {
						if (game_state[spc0][scr0][spc1][scr1] == 0) {
							continue;
						}

						if (current == A) {
						for (int roll = 3; roll < 10; roll++) {
							int new_spc = calc_position(spc0, roll);
							int new_scr = scr0 + new_spc;
							new_spc--;
							int factor = 1;

							if (roll == 4 || roll == 8) {
								factor = 3;
							} else if (roll == 5 || roll == 7) {
								factor = 6;
							} else if (roll == 6) {
								factor = 7;
							}

							if (new_scr >= 21) {
								won_0 += game_state[spc0][scr0][spc1][scr1] * factor;
								continue;
							}

							copy_state[new_spc][new_scr][spc1][scr1] += game_state[spc0][scr0][spc1][scr1] * factor;
						}

						current = B;
						}

						else if (current == B) {
						for (int roll = 3; roll < 10; roll++) {
							int new_spc = calc_position(spc1, roll);
							int new_scr = scr1 + new_spc;
							new_spc--;
							int factor = 1;

							if (roll == 4 || roll == 8) {
								factor = 3;
							} else if (roll == 5 || roll == 7) {
								factor = 6;
							} else if (roll == 6) {
								factor = 7;
							}

							if (new_scr >= 21) {
								won_1 += game_state[spc0][scr0][spc1][scr1] * factor;
								continue;
							}

							copy_state[spc0][scr0][new_spc][new_scr] += game_state[spc0][scr0][spc1][scr1] * factor;
						}

						current = A;
						}

					}
				}
			}
		}

		active_games = 0;

		for (int spc0 = 0; spc0 < 10; spc0++) {
			for (int scr0 = 0; scr0 < 21; scr0++) {
				for (int spc1 = 0; spc1 < 10; spc1++) {
					for (int scr1 = 0; scr1 < 21; scr1++) {
						game_state[spc0][scr0][spc1][scr1] = copy_state[spc0][scr0][spc1][scr1];
						copy_state[spc0][scr0][spc1][scr1] = 0;
						active_games += game_state[spc0][scr0][spc1][scr1] != 0;
					}
				}
			}
		}

	}

	gm->wins_1 = won_0;
	gm->wins_2 = won_1;
}
*/
