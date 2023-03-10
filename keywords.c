#include "utils.h"

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
	FILE *f_keywords; // fisier keywords
	f_keywords = fopen("data/keywords", "r");
	fscanf(f_keywords, "%d", nrkeywords);

	FILE *f_addkeywords; // fisier additional_keywords
	f_addkeywords = fopen("additional_keywords", "r");
	int nraddkeywords = 0; // numar additional_keywords
	fscanf(f_addkeywords, "%d", &nraddkeywords);

	keyword *keywordlist;
    /* initializeaza, aloca dinamic si citeste
    keywords din fisier */
	keywordlist = calloc(*nrkeywords, sizeof(keyword));
	for (int i = 0; i < *nrkeywords; i++) {
		keywordlist[i].app = calloc(nrfiles, sizeof(int));
		fscanf(f_keywords, "%s", keywordlist[i].word);
		keywordlist[i].blacklist = 0;
	}

    // adauga in lista de keywords cuvintele aditionale
	*nrkeywords += nraddkeywords;
	keyword *aux = realloc(keywordlist, (*nrkeywords) * sizeof(keyword));
	keywordlist = aux;
	for (int i = (*nrkeywords - nraddkeywords); i < *nrkeywords; i++) {
		keywordlist[i].app = calloc(nrfiles, sizeof(int));
		fscanf(f_addkeywords, "%s", keywordlist[i].word);
		fscanf(f_addkeywords, "%d", &keywordlist[i].blacklist);
	}

	fclose(f_keywords);
	fclose(f_addkeywords);

    // parcurge fiecare mail pt. a gasi keywords
	for (int j = 0; j < nrfiles; j++) {
		char path[PATH_MAX];
		snprintf(path, PATH_MAX, "data/emails/%d", j);
		FILE *f_email;
		f_email = fopen(path, "r");

		char currword[WORD_MAX]; // cuvantul curent
		// sare peste continut pana ajunge la sectiunea Body
		while (fscanf(f_email, "%s", currword) != EOF) {
			if (!strncmp(currword, "Body:", WORD_MAX))
				break;
		}

		/* parcurge fiecare cuvant din body si verifica daca
		acesta contine un keyword */
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

    // obtine stdev pt. fiecare keyword
	for (int i = 0; i < *nrkeywords; i++)
		keywordlist[i].stdev = get_stdev(keywordlist[i], nrfiles);

	return keywordlist;
}

void print_stat(int nrkeywords, keyword *keywordlist)
{
	FILE *f_stat = fopen("statistics.out", "w");

	for (int i = 0; i < DEFAULT_NRKWORD; i++) {
		fprintf(f_stat, "%s ", keywordlist[i].word);
		fprintf(f_stat, "%d ", keywordlist[i].count);
		fprintf(f_stat, "%.6f\n", keywordlist[i].stdev);
	}
	fclose(f_stat);
}
