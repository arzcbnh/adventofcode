// Advent of Code 2021 - Day 14
// Written by Henry Peaurt

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


//Types
typedef struct rule {
	char name[3];
	char result_a[3];
	char result_b[3];
	long count;
} rule;
typedef struct frequence {
	char name;
	long count;
} frequence;


// Function declarations
int getRules(rule ***rules);
rule *findRuleStruct(char *name, rule ***rules, int *rule_amount);
void stepPolymer(rule **rules, int rule_amount);
long subMostLessCommon(rule **rules, int rule_amount);
frequence *findAtomStruct(char name, frequence ***atoms, int *atom_amount);

// Inline functions
static inline void skipNewline(void) {
	getchar();
}
static inline void skipArrow(void) {
	getchar(); getchar(); getchar(); getchar();
}


int main(void)
{
	rule **rules = NULL;
	int rule_amount = getRules(&rules);
	
	for (int i = 0; i < 40; ++i) {
		if (i == 10)
			printf("Part 1: %ld\n", subMostLessCommon(rules,
								 rule_amount));
		stepPolymer(rules, rule_amount);
	}
	printf("Part 2: %ld\n", subMostLessCommon(rules, rule_amount));

	for (int i = 0; i < rule_amount; ++i)
		free(rules[i]);
	free(rules);
	return 0;
}


int getRules(rule ***rules)
{
	int rule_amount = 0;
	char name[3];
	rule *holder;

	name[2] = '\0';
	name[0] = getchar();
	while ((name[1] = getchar()) != '\n') {
		holder = findRuleStruct(name, rules, &rule_amount);
		holder->count++;
		name[0] = name[1];
	}
	skipNewline();

	while ((name[0] = getchar()) != EOF) {
		name[1] = getchar();
		skipArrow();
		holder = findRuleStruct(name, rules, &rule_amount);
		char c = getchar();
		holder->result_a[0] = name[0];
		holder->result_a[1] = holder->result_b[0] = c;
		holder->result_b[1] = name[1];
		skipNewline();
	}
	return rule_amount;
}


rule *findRuleStruct(char *name, rule ***rules, int *rule_amount)
{
	int i;

	for (i = 0; i < *rule_amount; ++i) {
		if (!strcmp(name, (*rules)[i]->name))
			return (*rules)[i];
	}
	*rules = realloc(*rules, sizeof(rule*) * (i + 1));
	(*rules)[i] = calloc(1, sizeof(rule));
	strcpy((*rules)[i]->name, name);
	(*rules)[i]->count = 0;
	(*rule_amount)++;
	return (*rules)[i];
}


void stepPolymer(rule **rules, int rule_amount)
{
	rule *holder = NULL, **holders = NULL;
	int holder_amount = 0;

	for (int i = 0; i < rule_amount; ++i) {
		if (rules[i]->count) {
		holder = findRuleStruct(rules[i]->result_a, &holders,
					&holder_amount);
		holder->count += rules[i]->count;
		holder = findRuleStruct(rules[i]->result_b, &holders,
					&holder_amount);
		holder->count += rules[i]->count;
		rules[i]->count = 0;
		}
	}
	// You can't remove the amount of stepped pairs before all steppings are
	// processed, since all atoms are inserted at the same time
	for (int i = 0; i < holder_amount; ++i) {
		holder = findRuleStruct(holders[i]->name, &rules,
					&rule_amount);
		holder->count = holders[i]->count;
	}

	for (int i = 0; i < holder_amount; ++i)
		free(holders[i]);
	free(holders);
}


long subMostLessCommon(rule **rules, int rule_amount)
{
	frequence *atom = NULL, **atoms = NULL;
	int atom_amount = 0;

	for (int i = 0; i < rule_amount; ++i) {
		atom = findAtomStruct(rules[i]->name[0], &atoms, &atom_amount);
		atom->count += rules[i]->count;
		atom = findAtomStruct(rules[i]->name[1], &atoms, &atom_amount);
		atom->count += rules[i]->count;
	}

	long most = 0;
	long less = LONG_MAX;

	for (int i = 0; i < atom_amount; ++i) {
		atom = atoms[i];
		atom->count = (atom->count + 1) / 2;
		most = atom->count > most ? atom->count : most;
		less = atom->count < less ? atom->count : less;
	}

	for (int i = 0; i < atom_amount; ++i)
		free(atoms[i]);
	free(atoms);
	return most - less;
}


frequence *findAtomStruct(char name, frequence ***atoms, int *atom_amount)
{
	int i;

	for (i = 0; i < *atom_amount; ++i) {
		if (name == (*atoms)[i]->name)
			return (*atoms)[i];
	}
	*atoms = realloc(*atoms, sizeof(frequence*) * (i + 1));
	(*atoms)[i] = malloc(sizeof(frequence));
	(*atoms)[i]->name = name;
	(*atoms)[i]->count = 0;
	(*atom_amount)++;
	return (*atoms)[i];
}
