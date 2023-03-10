#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

int main(void)
{
	int x = 0;
	char word[1000];

	FILE *spam = fopen("total_list", "r");
	FILE *ok = fopen("total_list_ok", "r");
	FILE *output = fopen("spam_words", "w");
	fscanf(spam, "%d", &x);
	while (fscanf(spam, "%s", word) != EOF) {
		bool found = false;

		int aux = 0;
		char straux[1000];
		fscanf(ok, "%d", &aux);
		while (fscanf(ok, "%s", straux) != EOF) {
			if (!strcasecmp(straux, word)) {
				found = true;
				break;
			}
			fscanf(ok, "%d", &aux);
		}
		if (found == false)
			fprintf(output, "%s %d\n", word, x);
		rewind(ok);
		fscanf(spam, "%d", &x);
	}
	fclose(spam);
	fclose(output);
	fclose(ok);
	return 0;
}

\ No newline at end of file
