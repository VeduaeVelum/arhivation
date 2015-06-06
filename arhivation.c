#include "ArhivationHeiders.h"

int arhivation(char * name)
{
	FILE *input_file, *binary_file;
	input_file = fopen(name, "rb");
	if (input_file == NULL)
	{
		puts("File didn't open. Maybe it isn't exist or it's empty\n");
		return -1;
	}
	binary_file = fopen("temp.txt", "wb");
	int ascii_input = 0;
	int unic_symbols_number = 0;
	struct hofman_tree simbols[CHAR_RANGE] = { 0 }; 
	struct hofman_tree *pointers_symbols[CHAR_RANGE]; 
	unic_symbols_number = calculation_occurrence_frequency(simbols, pointers_symbols, input_file);
	struct hofman_tree *root = make_tree(pointers_symbols, (unic_symbols_number - 1));
	make_codes(root);
	input_binary_file(unic_symbols_number, ascii_input, input_file, binary_file, simbols);
	fclose(input_file);
	make_arhivation_file(binary_file, name);
	return 0;
}
int calculation_occurrence_frequency(struct hofman_tree *simbols, struct hofman_tree **pointers_symbols, FILE *file1)
{
	FILE *occurancy_table_file;
	float occurrence_sum = 0;
	int ascii_input;
	int i = 0, j, unic_symbols_number = 0, unic_sings_number = 0,
		massive_unic_sings_number[CHAR_RANGE] = { 0 };
	struct hofman_tree current;
	occurancy_table_file = fopen("occurancy_table.txt", "wt");
	while ((ascii_input = fgetc(file1)) != EOF)
	{
		for (j = 0; j < CHAR_RANGE; j++)
		{
			if (ascii_input == simbols[j].symbol)
			{
				massive_unic_sings_number[j]++;
				unic_sings_number++;
				break;
			}
			if (simbols[j].symbol == 0)
			{
				simbols[j].symbol = (unsigned char)ascii_input;
				massive_unic_sings_number[j] = 1;
				unic_symbols_number++;
				unic_sings_number++;
				break;
			}
		}
	}
	while (i < unic_symbols_number)
	{
		simbols[i].freq = (float)(massive_unic_sings_number[i]);
		simbols[i].freq /= unic_sings_number;
		pointers_symbols[i] = &simbols[i++];
	}
	for (i = 1; i < unic_symbols_number; i++)
	{
		for (j = 0; j < (unic_symbols_number - 1); j++)
		{
			if (simbols[j].freq < simbols[j + 1].freq)
			{
				current = simbols[j];
				simbols[j] = simbols[j + 1];
				simbols[j + 1] = current;
			}
		}
	}
	for (i = 0; i < unic_symbols_number; i++)
	{
		occurrence_sum += simbols[i].freq;
		fprintf(occurancy_table_file, "Unic symbol = %d\tFrequency = %f\t\n", simbols[i].symbol, simbols[i].freq);
	}
	fprintf(occurancy_table_file, "\nWords = Summir = %f\n", occurrence_sum);
	fclose(occurancy_table_file);
	return unic_symbols_number;
}
void input_binary_file(int unic_symbols_number, int ascii_input, FILE *file1, FILE *file2, struct hofman_tree *symbols)
{
	int i;
	rewind(file1);
	while ((ascii_input = fgetc(file1)) != EOF)
	{
		for (i = 0; i < unic_symbols_number; i++)
		{
			if (ascii_input == symbols[i].symbol)
			{
				fputs(symbols[i].code_string, file2);
			}
		}
	}
	fputs("$", file2);
	fclose(file2);
}
struct hofman_tree *make_tree(struct hofman_tree *pointers_symbols[], int unic_symbols_number)//рeкурсивная функция создания дерева Хофмана
{
	int i, j;
	struct hofman_tree *current;
	current = (struct hofman_tree*)calloc(1, sizeof(struct hofman_tree));
	current->freq = pointers_symbols[unic_symbols_number]->freq + pointers_symbols[unic_symbols_number - 1]->freq;
	current->code_string[0] = 0;
	current->left = pointers_symbols[unic_symbols_number];
	current->right = pointers_symbols[unic_symbols_number - 1];
	if (unic_symbols_number == 1)	return current;
	else 
	{
		for (i = 0; i < unic_symbols_number; i++)
		{
			if (current->freq > pointers_symbols[i]->freq)
			{
				for (j = unic_symbols_number; j > i; j--)
				{
					pointers_symbols[j] = pointers_symbols[j - 1];
				}
				pointers_symbols[i] = current;
				break;
			}
		}
	}
	return make_tree(pointers_symbols, (unic_symbols_number - 1));
}
void make_codes(struct hofman_tree *root)
{
	if (root->left)
	{
		strcpy(root->left->code_string, root->code_string);
		strcat(root->left->code_string, "0");
		make_codes(root->left);
	}
	if (root->right)
	{
		strcpy(root->right->code_string, root->code_string);
		strcat(root->right->code_string, "1");
		make_codes(root->right);
	}
}
void make_arhivation_file(FILE *binary_file, char * name)
{
	bool flag0 = false, flag1 = false, flag_stop = false;
	char arhiv_name[ASCII_ONE + 1];
	FILE *arhivate_file;
	int i = 0, ascii_input, temp0 = 0, temp1 = 0;
	while (1)
	{
		arhiv_name[i] = name[i++];
		if (name[i - 1] == '.')
		{
			arhiv_name[i++] = 'b';
			arhiv_name[i++] = 'i';
			arhiv_name[i++] = 'n';
			arhiv_name[i++] = '\0';
			break;
		}
	}
	binary_file = fopen("temp.txt", "rb");
	arhivate_file = fopen(arhiv_name, "wt");
	while (flag_stop == false)
	{
		ascii_input = fgetc(binary_file);
		switch (ascii_input)
		{
			case ASCII_ZERO:
				if (flag1 == true)
				{
					fprintf(arhivate_file, "1x%d ", temp1);
					flag0 = true; flag1 = false;
					temp1 = 0;
					temp0++;
				}
				else
				{
					flag0 = true;
					temp0++;
				}
				break;
			case ASCII_ONE:
				if (flag0 == true)
				{
					fprintf(arhivate_file, "0x%d ", temp0);
					flag0 = false; flag1 = true;
					temp0 = 0;
					temp1++;
				}
				else
				{
					flag1 = true;
					temp1++;
				}
				break;
			default:
				if (flag0 == true) fprintf(arhivate_file, "0x%d ", temp0);
				if (flag1 == true) fprintf(arhivate_file, "1x%d ", temp1);
				flag_stop = true;
				fprintf(arhivate_file, "$ %s", name);
				break;
		}
	}
	fclose(binary_file);
	fclose(arhivate_file);
}
