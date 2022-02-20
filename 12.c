// Advent of Code 2021 - Day 12
// Written by Henry Peaurt

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>


// Types
typedef struct cave {
	char *name;
	bool is_small;
	struct cave **connections;
	int connection_amount;
} cave;


// Function declarations
int getCaves(cave ***caves);
int getCaveName(char **name);
cave *createCaveStruct(cave ***caves, int *cave_amount, char *name);
cave *checkExistingCaves(cave **caves, int cave_amount, char *name);
int findOncePaths(cave *current, cave** visited_address, int visited_amount);
int findTwicePaths(cave *current, cave** visited_address, int visited_amount,
		   bool visited_twice);
bool checkIfVisited(cave *current, cave **visited, int visited_amount);


int main(void)
{
	cave **caves = NULL;
	cave *start;
	int cave_amount;

	cave_amount = getCaves(&caves);
	start = checkExistingCaves(caves, cave_amount, "start");
	printf("Part 1: %d\n", findOncePaths(start, NULL, 0));
	printf("Part 2: %d\n", findTwicePaths(start, NULL, 0, false));

	for (int i = 0; i < cave_amount; ++i) {
		free(caves[i]->name);
		free(caves[i]->connections);
		free(caves[i]);
	}
	free(caves);
	return 0;
}


int getCaves(cave ***caves)
{
	cave *cave_a, *cave_b;
	char *name_a, *name_b;
	name_a = name_b = NULL;
	int cave_amount = 0;

	while (getCaveName(&name_a)) {
		getCaveName(&name_b);

		cave_a = createCaveStruct(caves, &cave_amount, name_a);
		cave_b = createCaveStruct(caves, &cave_amount, name_b);

		cave_a->connections = realloc(cave_a->connections, sizeof(cave*)
					     * (cave_a->connection_amount + 1));
		cave_b->connections = realloc(cave_b->connections, sizeof(cave*)
					     * (cave_b->connection_amount + 1));

		cave_a->connections[(cave_a->connection_amount)++] = cave_b;
		cave_b->connections[(cave_b->connection_amount)++] = cave_a;
	}
	free(name_a);
	free(name_b);
	return cave_amount;
}


int getCaveName(char **name)
{
	int i;
	char c;

	for (i = 0; (c = getchar()) != '-' && c != '\n' && c != EOF; ++i) {
		*name = realloc(*name, sizeof(char) * (i + 2));
		(*name)[i] = c;
	}
	(*name)[i] = '\0';
	return i;
}


cave *createCaveStruct(cave ***caves, int *cave_amount, char *name)
{
	cave *address = checkExistingCaves(*caves, *cave_amount, name);

	if (!address) {
		*caves = realloc(*caves, sizeof(cave*) * (*cave_amount + 1));
		(*caves)[*cave_amount] = malloc(sizeof(cave));
		address = (*caves)[(*cave_amount)++];
		// Initialization of the struct
		address->name = malloc(sizeof(char) * (strlen(name) + 1));
		strcpy(address->name, name);
		address->is_small = islower(name[0]);
		address->connections = NULL;
		address->connection_amount = 0;
	}
	return address;
}


cave *checkExistingCaves(cave **caves, int cave_amount, char *name)
{
	for (int i = 0; i < cave_amount; ++i) {
		if (!(strcmp(caves[i]->name, name)))
			return caves[i];
	}
	return NULL;
}


int findOncePaths(cave *current, cave** visited_address, int visited_amount)
{
	int found_paths = 0;
	cave** visited = malloc(sizeof(cave *) * visited_amount);
	memcpy(visited, visited_address, sizeof(cave *) * visited_amount);

	if (!(strcmp(current->name, "end"))) {
		free(visited);
		return 1;
	}
	if (current->is_small) {
		if (!(checkIfVisited(current, visited, visited_amount))) {
			visited = realloc(visited, sizeof(cave*) *
					  (visited_amount + 1));
			visited[visited_amount++] = current;
		} else {
			free(visited);
			return 0;
		}
	}

	for (int i = 0; i < current->connection_amount; ++i) {
		found_paths += findOncePaths(current->connections[i], visited,
					     visited_amount);
	}
	free(visited);
	return found_paths;
}


int findTwicePaths(cave *current, cave** visited_address, int visited_amount,
		   bool visited_twice)
{
	int found_paths = 0;
	cave** visited = malloc(sizeof(cave *) * visited_amount);
	memcpy(visited, visited_address, sizeof(cave *) * visited_amount);

	if (!(strcmp(current->name, "end"))) {
		free(visited);
		return 1;
	}
	if (current->is_small) {
		if (checkIfVisited(current, visited, visited_amount)) {
			if (visited_twice || 
			    (!(strcmp(current->name, "start")) &&
			    visited_address)) {
				free(visited);
				return 0;
			} else if (strcmp(current->name, "start")) {
				visited_twice = checkIfVisited(current, visited,
							       visited_amount);
			}
		} else {
			visited = realloc(visited, sizeof(cave*) *
					  (visited_amount + 1));
			visited[visited_amount++] = current;
		}
	}

	for (int i = 0; i < current->connection_amount; ++i) {
		found_paths += findTwicePaths(current->connections[i], visited,
					      visited_amount, visited_twice);
	}
	free(visited);
	return found_paths;
}


bool checkIfVisited(cave *current, cave **visited, int visited_amount)
{
	for (int i = 0; i < visited_amount; ++i) {
		if (current == visited[i])
			return true;
	}
	return false;
}
