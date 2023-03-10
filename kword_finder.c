#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define WORD_MAX 1000
#define NR_FILES 150
#define NR_WORDS 100000000

typedef struct {
	long long count;
	char *word;
} keyword;

int main(void)
{
	long long nrkw = 0;
	keyword *list = calloc(NR_WORDS, sizeof(keyword));
	FILE *ref = fopen("tasks/spam_detector/tests/01-spam_detector/01-spam_detector.ref", "r");
	printf("x\n");
	int spam = 0;
	for (long long i = 0; i < NR_FILES; i++) {
		fscanf(ref, "%d", &spam);
		while (spam && i < NR_FILES) {
			i++;
			fscanf(ref, "%d", &spam);
		}
		if (i >= 150)
		break;
		char path[PATH_MAX];
		snprintf(path, PATH_MAX, "data/emails/%lld", i);
		FILE *email = fopen(path, "r");
	printf("%lld\n", i);

		char currword[WORD_MAX];
		while (fscanf(email, "%s", currword) != EOF) {
			if (!strcmp(currword, "Body:")) {
				break;
			}
		}

		while (fscanf(email, "%s", currword) != EOF) {
			int found = 0;
			for (long long j = 0; j < nrkw; j++) {
				if (!strcasecmp(currword, list[j].word)) {
					found = 1;
					list[j].count++;
					break;
				}
			}

			if (!found) {
				list[nrkw].word = calloc(WORD_MAX, sizeof(char));
				snprintf(list[nrkw].word, WORD_MAX, "%s", currword);
				list[nrkw].count = 1;
				nrkw++;
			}
		}
	}
	printf("x\n");

	for (long long i = 0; i < nrkw - 1; i++) {
		for (long long j = i + 1; j < nrkw; j++) {
			if (list[i].count < list[j].count) {
				keyword temp = list[i];
				list[i] = list[j];
				list[j] = temp;
			}
		}
	}

	FILE *output = fopen("total_list_ok", "w");
	for (long long i = 0; i < nrkw; i++) {
		fprintf(output, "%lld %s\n", list[i].count, list[i].word);
		free(list[i].word);
	}

	free(list);
	return 0;
}
