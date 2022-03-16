// Advent of Code - Day 19
// Written by Henry Peaurt

// These programs are getting progressively worse. I tried to match scanners on the run, but the program was becoming
// so complicated I thought it was better and simpler to pregen the sets of rotated scanners.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "strmanip.h"
#include "memmanage.h"

// Types
typedef struct Beacon { int x, y, z; struct Beacon *next; } Beacon;
typedef struct Scanner {
	Beacon b[24];
	bool rotated;
	struct Scanner *next;
} Scanner;

// Function declarations
Scanner *input_scanners(void);
Beacon input_beacons(void);
Beacon dup_beacons(Beacon *start);
void rotate_beacons(Beacon *start, int x, int y, int z);
void rotate_scanner(Scanner *a, Scanner *b);
bool xor_rotated(Scanner *a, Scanner *b);
bool beacons_overlap(Beacon *a, Beacon *b, bool orient);
bool same_distance(Beacon *a1, Beacon *a2, Beacon *b1, Beacon *b2);
bool same_orientation(Beacon *a1, Beacon *a2, Beacon *b1, Beacon *b2);

#include <assert.h>

void
superrotate_scanner_whatever(Scanner *a, Scanner *b, Beacon *a1, Beacon *a2, Beacon *b1, Beacon *b2)
{
}

int
main(void)
{
	Scanner *start = input_scanners();
	start->rotated = true;

	for (Scanner *iter = start; iter != NULL; iter = iter->next)
	for (int j = 1; j < 24; j++) {
		int x = j % 4;
		int y = j / 4 % 4 * (j < 16);
		int z = (j >= 16) + (j >= 20) * 2;

		iter->b[j] = dup_beacons(&(iter->b[0]));
		rotate_beacons(&(iter->b[j]), x, y, z);
	}

	for (Scanner *i = start; i != NULL; i = i->next)
	for (Scanner *j = start; j != NULL; j = j->next)
		if (xor_rotated(i, j) && beacons_overlap(&(i->b[0]), &(j->b[0]), true))
		{
			if (!i->rotated) { Scanner *tmp = j; j = i; i = tmp; }
			rotate_scanner(i, j);

			for (Beacon *ai = &(i->b[0]);	   matches < 66 && ai != NULL; ai = ai->next)
			for (Beacon *aj = ai->next; matches < 66 && aj != NULL; aj = aj->next)
			for (Beacon *bi = &(j->b[0]);	   matches < 66 && bi != NULL; bi = bi->next)
			for (Beacon *bj = bi->next; matches < 66 && bj != NULL; bj = bj->next)
			{
				if (same_distance(ai, aj, bi, bj))
				{
					superrotate_scanner_whatever(i, j, ai, aj, bi, bj);
					goto out;
				}
			}
out:
		}

	/*
	assert(start->b[1].next->y == 409);
	assert(start->b[2].next->y == 643);
	assert(start->b[18].next->y == 528);
	*/

	/*
	for (int i = 0; i < 24; i++)
		printf("%i %i %i\n", start->b[i].x, start->b[i].y, start->b[i].z);
		*/

	/*
	assert(start->b[0].next->next->next->y == -675);
	assert(start->b[17].next->next->next->y == -675);
	assert(start->next->next->b[0].next->next->next->y == 584);
	assert(start->next->next->b[17].next->next->next->y == 584);
	*/

	/*
	Beacon a1, a2, b1, b2;
	a1.x = a1.y = a1.z = 4;
	a2.x = a2.y = a2.z = 10;
	b1.x = b1.y = b1.z = 6;
	b2.x = b2.y = b2.z = 12;
	assert(same_orientation(&a1, &a2, &b1, &b2));
	b1.x = b1.y = b1.z = 12;
	b2.x = b2.y = b2.z = 6;
	assert(same_orientation(&a1, &a2, &b1, &b2) == false);
	*/

	/*
	assert(scanners_overlap(start->next->next, start->next->next->next->next, false));
	assert(scanners_overlap(start->next->next, start->next->next->next->next, true) == false);
	*/

	return 0;
}

Scanner *
input_scanners(void)
{
	Scanner *start;
	Scanner **curr = &start;

	while (getchar() != EOF) {
		skip_lines(1);
		(*curr) = mem_alloc(sizeof(Scanner));
		(*curr)->b[0] = input_beacons();

		curr = &((*curr)->next);
	}

	return start;
}

Beacon
input_beacons(void)
{
	Beacon *start;
	Beacon **curr = &start;
	char *s;

	while (s = str_input()) {
		(*curr) = mem_alloc(sizeof(Beacon));

		(*curr)->x = atoi(str_word(0, s));
		(*curr)->y = atoi(str_word(1, s));
		(*curr)->z = atoi(str_word(2, s));

		curr = &((*curr)->next);
	}

	return *start;
}

Beacon
dup_beacons(Beacon *start)
{
	Beacon *new;
	Beacon **curr = &new;

	for (Beacon *i = start; i != NULL; i = i->next)	{
		*curr = mem_alloc(sizeof(Beacon));
		**curr = *i;
		curr = &((*curr)->next);
	}

	return *new;
}

void
rotate_beacons(Beacon *start, int x, int y, int z)
{
	for (Beacon *i = start; i != NULL; i = i->next) {
		int nx = i->x;
		int ny = i->y;
		int nz = i->z;

		for (int n = 0; n < x; n++) {
			int tmp = nz;
			nz = -ny;
			ny =  tmp;
		}

		for (int n = 0; n < y; n++) {
			int tmp = nx;
			nx = -nz;
			nz =  tmp;
		}

		for (int n = 0; n < z; n++) {
			int tmp = nx;
			nx = -ny;
			ny =  tmp;
		}

		i->x = nx;
		i->y = ny;
		i->z = nz;
	}
}

void
rotate_scanner(Scanner *a, Scanner *b)
{
	Scanner *rot  = a->rotated ? a : b;
	Scanner *nrot = a->rotated ? b : a;
	int i;

	for (i = 1; !beacons_overlap(&(rot->b[0]), &(nrot->b[i]), false); i++)
		;

	nrot->rotated = true;
	nrot->b[0] = nrot->b[i];
}

bool
xor_rotated(Scanner *a, Scanner *b)
{
	return !(a->rotated && b->rotated) && (a->rotated || b->rotated);
}

bool
beacons_overlap(Beacon *a, Beacon *b, bool dist)
{
	int matches = 0;

	for (Beacon *ai = a;	   matches < 66 && ai != NULL; ai = ai->next)
	for (Beacon *aj = a->next; matches < 66 && aj != NULL; aj = aj->next)
	for (Beacon *bi = b;	   matches < 66 && bi != NULL; bi = bi->next)
	for (Beacon *bj = b->next; matches < 66 && bj != NULL; bj = bj->next)
		matches += dist ? same_distance(ai, aj, bi, bj) : same_orientation(ai, aj, bi, bj);

	return matches >= 66;
}

bool
same_distance(Beacon *a1, Beacon *a2, Beacon *b1, Beacon *b2)
{
	int ax = abs(a1->x - a2->x);
	int ay = abs(a1->y - a2->y);
	int az = abs(a1->z - a2->z);

	int bx = abs(b1->x - b2->x);
	int by = abs(b1->y - b2->y);
	int bz = abs(b1->z - b2->z);

	return ax * ay * az == bx * by * bz;
}

bool
same_orientation(Beacon *a1, Beacon *a2, Beacon *b1, Beacon *b2)
{
	bool x = b1->x - a1->x + a2->x == b2->x;
	bool y = b1->y - a1->y + a2->y == b2->y;
	bool z = b1->z - a1->z + a2->z == b2->z;

	return x && y && z;
}

/*
RotationData
compare_scanners()
{

	
	
}
*/

/*
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
*/

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
