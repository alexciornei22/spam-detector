#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define WORD_MAX 1000
#define SPAM_SCORE 50.0 // prag-ul scorului pt. spam
#define DEFAULT_NRKWORD 20 // nr. default keywords
#define EMAILS_PATH "data/emails/"

// date specifice unui keyword
typedef struct {
	int *app; // nr. aparitii in fiecare email
	int count; // nr. total aparitii
	double stdev; // deviatia standard
	char word[WORD_MAX];
	int blacklist; // == 1 --> lista neagra
} keyword;

// criterii specifice scorului unui email
typedef struct {
	double kwords; // scor keywords
	int uppcase; // scor uppercase
	int spammers; // scor spammers
	bool re; // este reply
	bool adv; // este reclama
} crit;

// date despre un user spammer
typedef struct {
	char *usrname;
	int score;
} user;

// numara cate fisiere sunt intr-un director
int count_files(char *path);

// calculeaza stdev-ul unui keyword
double get_stdev(keyword currkeyword, int nrfiles);

// obtine datele pentru fiecare keyword
keyword *get_keywords(int nrfiles, int *nrkeywords);

// pune datele despre keywords in statistics.out
void print_stat(int nrkeywords, keyword *keywordlist);
