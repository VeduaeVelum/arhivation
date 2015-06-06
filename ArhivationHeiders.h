#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define SYMBOL_NUMBER 2
#define ASCII_ZERO 48
#define ASCII_ONE 49
#define ASCII_STOP 36
#define SIZEOF_BYTE 8
#define CHAR_RANGE 256


struct hofman_tree 
{
	unsigned char symbol;
	float freq;
	char code_string[CHAR_RANGE - 1];
	struct hofman_tree *left;
	struct hofman_tree *right;
};

int arhivation(char *);
struct hofman_tree *make_tree(struct hofman_tree *pointers_symbols[], int unic_symbols_number);
void make_codes(struct hofman_tree *root);
int calculation_occurrence_frequency(struct hofman_tree *simbols, struct hofman_tree **pointers_symbols, FILE *file1);
void input_binary_file(int unic_symbols_number, int ascii_input, FILE *file1, FILE *file2, struct hofman_tree *symbols);
void make_arhivation_file(FILE *binary_file, char *);
int dearhivation(char *);
