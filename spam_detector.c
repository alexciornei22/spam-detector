#include "utils.h"

// citeste lista de spammeri din fisierul corespunzator
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

/* calculeaza scorului unui anumit email
dupa formula si criteriile alese */
double get_score(crit critlist)
{
	double score = 0;
	score += 5 * critlist.kwords;
	score += 60 * critlist.uppcase;
	score += critlist.spammers;
	score -= critlist.re * 40;
	score += critlist.adv * 100;

	return score;
}

// analizeaza ficare email si returneaza lista cu scorurile acestora
double *scan_emails(int nrfiles, int nrkeywords, keyword *keywordlist)
{
	double *scores = calloc(nrfiles, sizeof(double)); // lista scoruri email
	crit *critlist = calloc(nrfiles, sizeof(crit)); // lista scoruri partiale
	int *lenlist = calloc(nrfiles, sizeof(int)); // lista lungimi body
	int nrspammers = 0;
	user *spammerlist = get_spammers(&nrspammers); // lista spammeri
	double avglen = 0; // medie lungimi body

	for (int i = 0; i < nrfiles; i++) { // parcurge fiecare email
		char path[PATH_MAX];
		snprintf(path, PATH_MAX, EMAILS_PATH "%d", i);
		FILE *f_email;
		f_email = fopen(path, "r");

		char currword[WORD_MAX]; // cuvantul curent
		char *sendername;
		while (fscanf(f_email, "%s", currword) != EOF) {
			if (!strcmp(currword, "Re:")) // verifica daca e reply
				critlist[i].re = true;
			if (!strcmp(currword, "ADV:")) // verifica daca e reclama
				critlist[i].adv = true;

			// verifica daca s-a gasit adresa sender-ului
			if (strchr(currword, '@') && currword[0] != '"') {
				if (currword[0] == '<') { // elimina "<>" daca e cazul
					sendername = currword + 1;
					sendername[strlen(sendername) - 1] = '\0';
				} else {
					sendername = currword;
				}
				break;
			}
		}
		// cauta adresa in lista de spammeri
		for (int j = 0; j < nrspammers; j++)
			if (!strcmp(sendername, spammerlist[j].usrname))
				critlist[i].spammers = spammerlist[j].score;

		// sare peste continut pana ajunge la Body
		while (fscanf(f_email, "%s", currword) != EOF) {
			if (!strcmp(currword, "Body:"))
				break;
		}

		int capscount = 0; // nr caractere uppercase
		while (fscanf(f_email, "%s", currword) != EOF) {
			if (!strcasecmp(currword, "groups"))
				critlist[i].re = true;
			int pos = 0;
			while (currword[pos] != '\0') { // numara caracterele cuvantului
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
	for (int i = 0; i < nrfiles; i++) // calcul scor keywords
		for (int j = 0; j < nrkeywords; j++) {
			critlist[i].kwords += keywordlist[j].app[i] * keywordlist[j].stdev *
				(avglen / lenlist[i]);
			if (keywordlist[j].app[i])
				critlist[i].kwords += keywordlist[j].blacklist * 100;
		}

	for (int i = 0; i < nrfiles; i++)
		scores[i] = get_score(critlist[i]);

	free(critlist);
	free(lenlist);
	for (int i = 0; i < nrspammers; i++)
		free(spammerlist[i].usrname);
	free(spammerlist);
	return scores;
}

// scrie rezultatele predictiei in fisier
void print_pred(double *scores, int nrfiles)
{
	FILE *f_predict;
	f_predict = fopen("prediction.out", "w");

	for (int i = 0; i < nrfiles; i++) {
		if (scores[i] > SPAM_SCORE)
			fprintf(f_predict, "1\n");
		else
			fprintf(f_predict, "0\n");
	}

	fclose(f_predict);
}

int main(void)
{
	int nrfiles = count_files(EMAILS_PATH);
	int nrkeywords = 0;
	keyword *keywordlist = get_keywords(nrfiles, &nrkeywords);
	print_stat(nrkeywords, keywordlist);

	double *scores = scan_emails(nrfiles, nrkeywords, keywordlist);
	print_pred(scores, nrfiles);

	for (int i = 0; i < nrkeywords; i++)
		free(keywordlist[i].app);
	free(keywordlist);
	free(scores);
	return 0;
}
