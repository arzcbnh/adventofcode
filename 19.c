// Advent of Code - Day 19
// Written by Henry Peaurt

// These programs are getting progressively worse. I tried to match scanners on the run, but the program was becoming
// so complicated I thought it was better and simpler to pregen the sets of rotated scanners.

#include <stdio.h>
#include <stdlib.h>

#include "strmanip.h"
#include "memmanage.h"

/*
// Enums
enum axis { NONE, X, Y, Z };

// Types
typedef coords beacon;

typedef struct {
	beacon* a;
	beacon* b;
} beacon_pair;

typedef struct {
	enum axis ax;
	int degrees;
} rotation_info;
*/

typedef struct {
	int x, y, z;
} beacon;

typedef struct {
	int x, y, z;

	beacon** beacon_list[24];
	int beacon_cnt;
} scanner;

typedef struct {
	scanner** scanner_list;
	int scanner_cnt;
} cntxt;

// Function declarations
void input_scanners(cntxt* c);
void input_beacons(scanner* scn);
void gen_rotated_sets(scanner* scn);

int
main(void)
{
	cntxt c = {
		.scanner_list = NULL,
		.scanner_cnt = 0,
	};

	input_scanners(&c);

	for (int i = 1; i < c.scanner_cnt; i++) {
		gen_rotated_sets(c.scanner_list[i]);
	}

	/*
	for (int i = 0; i < c.scanner_cnt; i++) {
		printf("--- scanner %i ---\n", i);

		for (int j = 0; j < c.scanner_list[i]->b_cnt; j++) {
			printf("%i, %i, %i\n", c.scanner_list[i]->b_list[j]->x, c.scanner_list[i]->b_list[j]->y, c.scanner_list[i]->b_list[j]->z);
		}

		putchar('\n');
	}
	*/

	/*
	c.beacon_list = c.scanner_list[0]->b_list;
	c.beacon_cnt = c.scanner_list[0]->b_cnt;
	c.beacon_cap
	*/
	/*
	find_beacons(&c);
	*/
	/*
	find_scanner_positions(&c);
	*/

	return 0;
}

void
input_scanners(cntxt* c)
{
	scanner** list = mem_alloc(32 * sizeof(scanner*));
	int cnt = 0;
	int cap = 32;

	char* s = str_input();

	while (s != NULL) {
		if (cnt >= cap) {
			cap += 32;
			list = mem_realloc(list, cap * sizeof(scanner*));
		}

		scanner* scn = mem_alloc(sizeof(scanner));
		input_beacons(scn);

		list[cnt] = scn;
		cnt++;

		s = str_input();
	}

	c->scanner_list = list;
	c->scanner_cnt = cnt;
}

void
input_beacons(scanner* scn)
{
	beacon** list = mem_alloc(32 * sizeof(beacon*));
	int cnt = 0;
	int cap = 32;

	char* s = str_input();

	while (s != NULL) {
		if (cnt >= cap) {
			cap += 32;
			list = mem_realloc(list, cap * sizeof(beacon*));
		}

		beacon* b = mem_alloc(sizeof(beacon));

		b->x = atoi(str_word(0, s));
		b->y = atoi(str_word(1, s));
		b->z = atoi(str_word(2, s));

		list[cnt] = b;
		cnt++;

		s = str_input();
	}

	scn->beacon_list[0] = list;
	scn->beacon_cnt = cnt;
}

void
gen_rotated_sets(scanner* scn)
{
	rotate_plane(X, Y);
	gen_rotated_sets(scn, 1);
	rotate_plane(X, Y);
	gen_rotated_sets(scn, 2);
	rotate_plane(X, Y);
	gen_rotated_sets(scn, 3);

	invert_front_axis(scn);
	gen_rotated_sets(scn, 4);
	rotate_plane(X, Y);
	gen_rotated_sets(scn, 5);
	rotate_plane(X, Y);
	gen_rotated_sets(scn, 6);
	rotate_plane(X, Y);
	gen_rotated_sets(scn, 7);

	gen_rotated_sets(scn, 8);
	rotate_plane(X, Y);
	gen_rotated_sets(scn, 9);
	rotate_plane(X, Y);
	gen_rotated_sets(scn, 10);
	rotate_plane(X, Y);
	gen_rotated_sets(scn, 11);

	invert_front_axis(scn);
	gen_rotated_sets(scn, 12);
	rotate_plane(X, Y);
	gen_rotated_sets(scn, 13);
	rotate_plane(X, Y);
	gen_rotated_sets(scn, 14);
	rotate_plane(X, Y);
	gen_rotated_sets(scn, 15);

	gen_rotated_sets(scn, 16);
	rotate_plane(X, Y);
	gen_rotated_sets(scn, 17);
	rotate_plane(X, Y);
	gen_rotated_sets(scn, 18);
	rotate_plane(X, Y);
	gen_rotated_sets(scn, 19);
	
	invert_front_axis(scn);
	gen_rotated_sets(scn, 20);
	rotate_plane(X, Y);
	gen_rotated_sets(scn, 21);
	rotate_plane(X, Y);
	gen_rotated_sets(scn, 22);
	rotate_plane(X, Y);
	gen_rotated_sets(scn, 23);
}

rotate_plane(axis a, axis b)
{
	for (int i = 0; i < c.
}

gen_rotated_sets(scanner* scn, int index)
{
}

/*
void
orient_scanner(cntxt* c, scanner* scn)
{
	for (int i = 0; i < c->beacon_cnt - 1; i++) {
		for (int j = i + 1; j < c->beacon_cnt - 1; j++) {
			long dist = measure_distance(c->beacon_list[i], c->beacon_list[j]);
			beacon_pair* bp = find_matching_pair(dist, scn);

			if (bp == NULL) { continue; }

			beacon b_stack[4] = {
				c->beacon_list[i],
				c->beacon_list[j],
				bp->a,
				bp->b
			};

			rotation_info* rotinf = calc_orientation(b_stack);
		}
	}
}

rotation_info*
calc_orientation(beacon b_stack[4])
{
	rotation_info* rotinf = mem_alloc(sizeof(rotation_info));

	rotate_x(b_stack, rotinf);

	if (rotinf->ax != NONE) {
		return rotinf;
	}

	rotate_y(b_stack, rotinf);

	if (rotinf->ax != NONE) {
		return rotinf;
	}

	rotate_z(b-stack, rotinf);

	if (rotinf->ax != NONE) {
		puts("EROOR");
		return rotinf;
	}
	return rotinf;
}

void
rotate_x(beacon b_stack[4], rotation_info* rotinf)
{
	const int center_z = b_stack[2]->z;
	const int center_y = b_stack[2]->y;
	int curr_z = b_stack[3]->z;
	int curr_y = b_stack[3]->y;

	for (int rot_x = 90; rot_x <= 360; rot_x += 90) {
		new_z = center_z + center_y - curr_y;
		new_y = center_y - center_z + curr_z;
		curr_z = new_z;
		curr_y = new_y;

		if (
			b_stack[1]->z - b_stack[0]->z + center_z == curr_z &&
			b_stack[1]->y - b_stack[0]->y + center_y == curr_y
		   ) {
			rotinf->ax = X;
			rotinf->degrees = rot_x;
			return;
		}
	}
}

void
rotate_y(beacon b_stack[4], rotation_info* rotinf)
{
	const int center_z = b_stack[2]->z;
	const int center_y = b_stack[2]->y;
	int curr_z = b_stack[3]->z;
	int curr_y = b_stack[3]->y;

	for (int rot_x = 90; rot_x <= 360; rot_x += 90) {
		new_z = center_z + center_y - curr_y;
		new_y = center_y - center_z + curr_z;
		curr_z = new_z;
		curr_y = new_y;

		if (
			b_stack[1]->z - b_stack[0]->z + center_z == curr_z &&
			b_stack[1]->y - b_stack[0]->y + center_y == curr_y
		   ) {
			rotinf->ax = X;
			rotinf->degrees = rot_x;
			return;
		}
	}
}

void
rotate_x(beacon b_stack[4], rotation_info* rotinf)
{
	const int center_z = b_stack[2]->z;
	const int center_y = b_stack[2]->y;
	int curr_z = b_stack[3]->z;
	int curr_y = b_stack[3]->y;

	for (int rot_x = 90; rot_x <= 360; rot_x += 90) {
		new_z = center_z + center_y - curr_y;
		new_y = center_y - center_z + curr_z;
		curr_z = new_z;
		curr_y = new_y;

		if (
			b_stack[1]->z - b_stack[0]->z + center_z == curr_z &&
			b_stack[1]->y - b_stack[0]->y + center_y == curr_y
		   ) {
			rotinf->ax = X;
			rotinf->degrees = rot_x;
			return;
		}
	}
}

	for (int rot_x = 0; rot_x <= 360; rot_x += 90) {
		a1->x 
		for (int rot_y = 0; rot_y <= 360; rot_y += 90) {
			for (int rot_z = 0; rot_z= < 360; rot_z += 90) {
				if (
					a2->x + b1->x - a1->x != b2->x &&
					a2->y + b1->y - a1->y != b2->y &&
					a2->z + b1->z - a1->z != b2->z
				   ) {
					orientation_data od = {
						.rot_x = rot_x,
						.rot_y = rot_y,
						.rot_z = rot_z
					};

					return od;
				}
			}
		}
	}

	orientation_data od = {
		.rot_x = -1,
		.rot_y = -1,
		.rot_z = -1
	};

	return od;
}

long
measure_distance(beacon* a, beacon* b)
{
	int x = a->x - b->x;
	x *= x > 0 ? 1 : -1;
	int y = a->y - b->y;
	y *= y > 0 ? 1 : -1;
	int z = a->z - b->z;
	z *= z > 0 ? 1 : -1;

	// Not the actual distance, but there's no need to calculate square root.
	return x * x + y * y + z * z;
}

beacon_pair*
find_matching_pair(long dist, scanner* scn)
{
	for (int i = 0; i < scn->b_count - 1; i++) {
		for (int j = 0; j < scn->b_count; j++) {
			if (measure_distance(scn->b_list[i], scn->b_list[j]) == dist) {
				beacon_pair bp = mem_alloc(sizeof(beacon_pair));
				bp->a = scn->b_list[i];
				bp->b = scn->b_list[j];

				return bp;
			}
		}
	}

	return NULL;
}

void
match_beacons(beacon b[4])
{
}
*/

/*
void
find_scanner_positions(cntxt* c)
{
	for (int i = 0; i < c->scanner_cnt; i++) {
		for (int j = 1; j < c->scanner_cnt; j++) {
			measure_against(c->scanner_list, c->scanner_cnt);
		}
	}
}
*/
