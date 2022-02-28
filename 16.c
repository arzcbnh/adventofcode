// Advent of Code 2021 - Day 15
// Written by Henry Peaurt

// I like to think this program turned out long because I also made it easy to understand, but I'm not sure. I think
// the logic is quite simple, although there's lot of math being done on the spot. You have to comprehend how the data
// is organized beforehand.

#include <stdio.h>
#include <string.h>

#include "strmanip.h"
#include "memmanage.h"

// Constants
const char* BINARY_STR[16] = {
	"0000",	"0001",	"0010",	"0011",
	"0100",	"0101",	"0110",	"0111",
	"1000",	"1001",	"1010",	"1011",
	"1100",	"1101",	"1110",	"1111"
};

// Types
typedef enum {
	SUM,
	PRODUCT,
	MINIMUM,
	MAXIMUM,
	LITERAL,
	GREATER_THAN,
	LESS_THAN,
	EQUAL_TO,

	TOTAL_LEN = 0,
	PKT_NUM = 1
} packet_type;

typedef struct packet {
	char* bin;
	int version;
	packet_type type_id;

	struct packet** children;
	int cnt;
} packet;

// Function declarations
char* input_bin(void);
packet* read_header(char* bin);
packet* read_literal(char* bin, int start);
packet* read_op_15(char* bin, int start);
packet* read_op_11(char* bin, int start);
packet* alloc_packet(char* pbin, packet** children, int cnt);
int measure_child_len(packet* p);
int sum_versions(packet* main);
long calc_packets(packet* p);
long decode_literal(char* pbin);
long sum_array(long* nums, int n_cnt);
long mtp_array(long* nums, int n_cnt);
long min_array(long* nums, int n_cnt);
long max_array(long* nums, int n_cnt);

int
main(void)
{
	char* binary = input_bin();
	packet* start_pkt = read_header(binary);

	printf("Part 1: %u\n", sum_versions(start_pkt));
	printf("Part 2: %lu\n", calc_packets(start_pkt));

	mem_clean();

	return 0;
}


char*
input_bin(void)
{
	char* bin = NULL;
	char a = getchar();

	while (a != '\n') {
		int n = chr_hex_to_int(a);
		const char* s = BINARY_STR[n];

		for (int i = 0; i < 4; ++i)
			bin = str_push(bin, s[i]);

		a = getchar();
	}

	return bin;
}

packet*
read_header(char* bin)
{
	static int index;
	int type_id = str_bin_to_int(str_extract(bin, index + 3, index + 6));
	int length = bin[index + 6] - '0'; // Won't even be used if type_id is 4
	packet* p = NULL;

	/*
	printf("%i %i %i\n", start, type_id, length);
	*/

	if (type_id == LITERAL) {
		p = read_literal(bin, index);
		index += strlen(p->bin);
	} else if (length == TOTAL_LEN) {
		index += 22;
		p = read_op_15(bin, index - 22); // The reason it subs the index after adding up is for ease of reading
						 // what the function is doing
	} else if (length == PKT_NUM) {
		index += 18;
		p = read_op_11(bin, index - 18);
	}

	return p;
}

packet*
read_literal(char* bin, int start)
{
	int len = start + 6;

	do {
		len += 5;
	} while (bin[len - 5] != '0');

	return alloc_packet(str_extract(bin, start, len), NULL, 0);
}

packet*
read_op_15(char* bin, int start)
{
	int len_packets = str_bin_to_int(str_extract(bin, start + 7, start + 22));
	int len_measured = 0;

	packet** chld = NULL;
	int cnt = 0;

	for (;len_measured < len_packets - 7; cnt++) {
		chld = mem_realloc(chld, (cnt + 1) * sizeof(packet*));

		chld[cnt] = read_header(bin);
		len_measured += measure_child_len(chld[cnt]);
	}

	return alloc_packet(str_extract(bin, start, start + 22), chld, cnt);
}

packet*
read_op_11(char* bin, int start)
{
	int sub_packets = str_bin_to_int(str_extract(bin, start + 7, start + 18));
	packet** chld = NULL;
	int cnt = 0;

	for (; cnt < sub_packets; cnt++) {
		chld = mem_realloc(chld, (cnt + 1) * sizeof(packet*));
		chld[cnt] = read_header(bin);
	}

	return alloc_packet(str_extract(bin, start, start + 18), chld, cnt);
}

packet*
alloc_packet(char* pbin, packet** children, int cnt)
{
	packet* p = mem_alloc(sizeof(packet));

	p->bin = pbin;
	p->version = str_bin_to_int(str_extract(pbin, 0, 3));
	p->type_id = str_bin_to_int(str_extract(pbin, 3, 6));
	p->children = children;
	p->cnt = cnt;

	return p;
}

int
measure_child_len(packet* p)
{
	int len = strlen(p->bin);

	for (int i = 0; i < p->cnt; i++)
		len += measure_child_len(p->children[i]);

	return len;
}

int
sum_versions(packet* p)
{
	int sum = p->version;

	for (int i = 0; i < p->cnt; i++)
		sum += sum_versions(p->children[i]);

	return sum;
}

long
calc_packets(packet* p)
{
	long* nums = NULL;
	int n_cnt = 0;

	if (p->type_id == LITERAL) {
		return decode_literal(p->bin);
	} else for (; n_cnt < p->cnt; n_cnt++) {
			nums = mem_realloc(nums, (n_cnt + 1) * sizeof(long));
			nums[n_cnt] = calc_packets(p->children[n_cnt]);
	}

	switch (p->type_id) {
	case SUM:
		return sum_array(nums, n_cnt);
	case PRODUCT:
		return mtp_array(nums, n_cnt);
	case MINIMUM:
		return min_array(nums, n_cnt);
	case MAXIMUM:
		return max_array(nums, n_cnt);
	case LITERAL:
		return 0; // The compiler forces me to include this one
	case GREATER_THAN:
		return nums[0] > nums[1];
	case LESS_THAN:
		return nums[0] < nums[1];
	case EQUAL_TO:
		return nums[0] == nums[1];
	}

	return 0;
}

long
decode_literal(char* pbin)
{
	int index = 1;
	long num = 0;

	do {
		index += 5;
		num <<= 4;
		num |= str_bin_to_int(str_extract(pbin, index + 1, index + 5));
	} while (pbin[index] != '0');

	return num;
}

long
sum_array(long* nums, int n_cnt)
{
	long r = 0;

	for (int i = 0; i < n_cnt; i++)
		r += nums[i];

	return r;
}

long
mtp_array(long* nums, int n_cnt)
{
	long r = 1;

	for (int i = 0; i < n_cnt; i++)
		r *= nums[i];

	return r;
}

long
min_array(long* nums, int n_cnt)
{
	long min = nums[0];

	for (int i = 1; i < n_cnt; i++)
		min = min < nums[i] ? min : nums[i];

	return min;
}

long
max_array(long* nums, int n_cnt)
{
	long max = nums[0];

	for (int i = 1; i < n_cnt; i++)
		max = max > nums[i] ? max : nums[i];

	return max;
}
