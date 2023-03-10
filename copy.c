#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define WORD_MAX 1000
#define SPAM_SCORE 35.0
#define DEFAULT_KWORD 20

typedef struct {
	int *app;
	int count;
	double stdev;
	char word[WORD_MAX];
} keyword;

typedef struct {
	double kwords;
	int uppcase;
	int spammers;
	bool re;
	bool adv;
} crit;

typedef struct {
	char *usrname;
	int score;
} user;

int count_files(char *path)
{
	int nrfiles = 0;
	DIR *email_dir;
	struct dirent *email_entry;

	email_dir = opendir(path);
	email_entry = readdir(email_dir);
	while (email_entry) {
		if (email_entry->d_type == DT_REG)
			nrfiles++;
		email_entry = readdir(email_dir);
	}

	closedir(email_dir);
	return nrfiles;
}

double get_stdev(keyword currkeyword, int nrfiles)
{
    // calculeaza media aritmetica
	double arith_mean = 0;
	for (int i = 0; i < nrfiles; i++)
		arith_mean += currkeyword.app[i];
	arith_mean /= nrfiles;

    // calculeaza stdev conform formulei
	double stdev = 0;
	for (int i = 0; i < nrfiles; i++)
		stdev += pow(currkeyword.app[i] - arith_mean, 2);
	stdev /= nrfiles;
	stdev = sqrt(stdev);

	return stdev;
}

keyword *get_keywords(int nrfiles, int *nrkeywords)
{
	FILE *f_keywords;
	f_keywords = fopen("data/keywords", "r");
	fscanf(f_keywords, "%d", nrkeywords);

	FILE *f_addkeywords;
	f_addkeywords = fopen("additional_keywords", "r");
	int nraddkeywords = 0;
	fscanf(f_addkeywords, "%d", &nraddkeywords);

	keyword *keywordlist;
	keywordlist = calloc(*nrkeywords, sizeof(keyword));
	for (int i = 0; i < *nrkeywords; i++) {
		keywordlist[i].app = calloc(nrfiles, sizeof(int));
		fscanf(f_keywords, "%s", keywordlist[i].word);
		keywordlist[i].count = 0;
	}

	*nrkeywords += nraddkeywords;
	keyword *aux = realloc(keywordlist, (*nrkeywords) * sizeof(keyword));
	keywordlist = aux;
	for (int i = (*nrkeywords - nraddkeywords); i < *nrkeywords; i++) {
		keywordlist[i].app = calloc(nrfiles, sizeof(int));
		fscanf(f_addkeywords, "%s", keywordlist[i].word);
		keywordlist[i].count = 0;
	}

	for (int j = 0; j < nrfiles; j++) {
		char path[PATH_MAX];
		snprintf(path, PATH_MAX, "data/emails/%d", j);
		FILE *f_email;
		f_email = fopen(path, "r");

		char currword[WORD_MAX];
		while (fscanf(f_email, "%s", currword) != EOF) {
			if (!strncmp(currword, "Body:", WORD_MAX))
				break;
		}

		while (fscanf(f_email, "%s", currword) != EOF) {
			for (int i = 0; i < *nrkeywords; i++) {
				char *p = strcasestr(currword, keywordlist[i].word);
			    while (p) {
					keywordlist[i].app[j]++;
					keywordlist[i].count++;
					p = strcasestr(p + 1, keywordlist[i].word);
			    }
			}
		}
		fclose(f_email);
	}

	for (int i = 0; i < *nrkeywords; i++)
		keywordlist[i].stdev = get_stdev(keywordlist[i], nrfiles);
	fclose(f_keywords);
	return keywordlist;
}

void print_stat(int nrkeywords, keyword *keywordlist)
{
	FILE *f_stat = fopen("statistics.out", "w");

	for (int i = 0; i < DEFAULT_KWORD; i++) {
		fprintf(f_stat, "%s ", keywordlist[i].word);
		fprintf(f_stat, "%d ", keywordlist[i].count);
		fprintf(f_stat, "%.6f\n", keywordlist[i].stdev);
	}
	fclose(f_stat);
}

user *get_spammers(int *nrspammers)
{
	FILE *f_spammers;
	f_spammers = fopen("data/spammers", "r");
	fscanf(f_spammers, "%d", nrspammers);

	user *spammerlist = calloc(*nrspammers, sizeof(user));
	for (int i = 0; i < *nrspammers; i++) {
		spammerlist[i].usrname = calloc(WORD_MAX, sizeof(char));
		fscanf(f_spammers, "%s", spammerlist[i].usrname);
		fscanf(f_spammers, "%d", &spammerlist[i].score);
	}

	fclose(f_spammers);
	return spammerlist;
}

double *scan_emails(int nrfiles, int nrkeywords, keyword *keywordlist)
{
	double *scores = calloc(nrfiles, sizeof(double));
	crit *critlist = calloc(nrfiles, sizeof(crit));
	int *lenlist = calloc(nrfiles, sizeof(int));
	int nrspammers = 0;
	user *spammerlist = get_spammers(&nrspammers);
	double avglen = 0;

	for (int i = 0; i < nrfiles; i++) {
		char path[PATH_MAX];
		snprintf(path, PATH_MAX, "data/emails/%d", i);
		FILE *f_email;
		f_email = fopen(path, "r");

		char currword[WORD_MAX];
		char *sendername;
		while (fscanf(f_email, "%s", currword) != EOF) {
			if (!strcasecmp(currword, "Re:"))
				critlist[i].re = true;
			if (!strcasecmp(currword, "ADV:"))
				critlist[i].adv = true;
			if (strchr(currword, '@') && currword[0] != '"') {
				if (currword[0] == '<') {
					sendername = currword + 1;
					sendername[strlen(sendername) - 1] = '\0';
				} else {
					sendername = currword;
				}
				break;
			}
		}
		for (int j = 0; j < nrspammers; j++)
			if (!strcmp(sendername, spammerlist[j].usrname))
				critlist[i].spammers = spammerlist[j].score;

		while (fscanf(f_email, "%s", currword) != EOF) {
			if (!strncmp(currword, "Body:", WORD_MAX))
				break;
		}
		int capscount = 0;
		while (fscanf(f_email, "%s", currword) != EOF) {
			if (!strcasecmp(currword, "Re:"))
				critlist[i].re = true;
			if (!strcasecmp(currword, "groups"))
				critlist[i].re = true;

			int pos = 0;
			while (currword[pos] != '\0') {
				lenlist[i]++;
				if (currword[pos] >= 'A' && currword[pos] <= 'Z')
					capscount++;
				pos++;
			}
		}

		avglen += lenlist[i];
		critlist[i].uppcase = (capscount > lenlist[i] / 2) ? 1 : 0;

		fclose(f_email);
	}

	avglen /= nrfiles;
	int kwfound[150] = {0};
	for (int i = 0; i < nrfiles; i++)
		for (int j = 0; j < nrkeywords; j++) {
			critlist[i].kwords += keywordlist[j].app[i] * keywordlist[j].stdev * (avglen / lenlist[i]);
			if (keywordlist[j].app[i] != 0)
				kwfound[i]++;
		}

	for (int i = 0; i < nrfiles; i++) {
		scores[i] += 5 * critlist[i].kwords;
		scores[i] += 30 * critlist[i].uppcase;
		scores[i] += 5 * critlist[i].spammers;
		if (critlist[i].re)
			scores[i] -= 40;
		if (critlist[i].adv)
			scores[i] += 100;
		if (kwfound[i] <= 2)
			scores[i] -= 15;
	}

	int test = 10;
	printf("%f ", critlist[test].kwords);
	for (int j = 0; j < nrkeywords; j++)
		if (keywordlist[j].app[test])
			printf("%s %d %f\n", keywordlist[j].word, keywordlist[j].app[test], keywordlist[j].stdev);
	printf("%d ", critlist[test].uppcase);
	printf("%d ", critlist[test].spammers);
	printf("%d ", critlist[test].re);
	printf("%f\n", scores[test]);

	free(critlist);
	free(lenlist);
	for (int i = 0; i < nrspammers; i++)
		free(spammerlist[i].usrname);
	free(spammerlist);
	return scores;
}

int main(void)
{
	int nrfiles = count_files("data/emails");
	int nrkeywords = 0;
	keyword *keywordlist = get_keywords(nrfiles, &nrkeywords);
	print_stat(nrkeywords, keywordlist);

	FILE *f_predict;
	f_predict = fopen("prediction.out", "w");
	double *scores = scan_emails(nrfiles, nrkeywords, keywordlist);

	for (int i = 0; i < nrfiles; i++) {
		if (scores[i] > SPAM_SCORE)
			fprintf(f_predict, "1\n");
		else
			fprintf(f_predict, "0\n");
	}

	for (int i = 0; i < nrkeywords; i++) {
		free(keywordlist[i].app);
	}
	free(keywordlist);
	free(scores);
	fclose(f_predict);
	return 0;
}
