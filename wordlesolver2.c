#include <stdio.h>
int main(int argc, char* argv[]) {
	// read word list
	int wlistlen = 14000;
	int wlen = -1;
	if (argc == 2) {
		wlen = 0;
		char c;
		int i = 0;
		while ((c = *(argv[1] + i)) != '\0') {
			if (c > 47 && c < 58) wlen = 10 * wlen + c - 48;
			else {
				wlen = -1;
				break;
			}
			i++;
		}
	}
	if (wlen < 0){
		printf("Please enter word length: ");
		scanf("%d", &wlen);
		getchar();
		putchar('\n');
	}
	char wordlist[wlistlen][wlen + 1];
	FILE* wfile = fopen("wordlist-english0.txt", "r");
	for (int i = 0; i < wlistlen; i++) {
		char c; 
		int k;
		for (k = 0; (c = fgetc(wfile)) != '\n'; k++) {
			if (c == EOF) {
				wlistlen = i - 2;
				break;
			}
			if (k < wlen) {
				if (c > 96 && c < 123) c -= 32;
				if (c < 65 || c > 90) k = wlen + 1;
				wordlist[i][k] = c;
			}
		}
		if (k != wlen) i--;
		else wordlist[i][wlen] = '\0';
	}
	fclose(wfile);
	if (wlistlen != 14000) wlistlen += 2;
	printf("Number of available words: %d\n", wlistlen);
	
	int unknown = wlen;
	int trycount;
	int num[26] = {0};
	char word[wlen + 1];
	char tries[wlen * 3 / 2 + 5][wlen + 1];
	for(int i = 0; i < wlen; i++) word[i] = '*';
	word[wlen] = '\0';
	printf("please enter your guess, followed by its score, separated by a line break:\n");
	for (trycount = 0; trycount < wlen * 3 / 2 + 5 && unknown; trycount++) {
		printf("new try\n");
		int tmpnum[26] = {0};
		char try[wlen + 1];
		char score[wlen + 1];
		int i;
		for (i = 0; i <= wlen && (try[i] = getchar()) != '\n'; i++) if (try[i] > 96 && try[i] < 123) try[i] -= 32;
		if (i == wlen + 1 && try[wlen] != '\n') {
			while ((score[0] = getchar()) != '\n');
			printf("Too long!\n");
			trycount--;
			continue;
		} else if (i < wlen) {
			printf("Too short!\n");
			trycount--;
			continue;
		}
		for (i = 0; i < wlen; i++) tries[trycount][i] = try[i];
		tries[trycount][wlen] = '\0';
		for (i = 0; i <= wlen && (score[i] = getchar()) != '\n'; i++) {
			if (score[i] == 'x') score[i] = 'X';
			else if (score[i] == '_') score[i] = '-';
			else if (score[i] == '+'
			      || score[i] == '.') score[i] = '*';
			else if (score[i] != 'X' && score[i] != '*' && score[i] != '-') {
				printf("Invalid scoring caracter, use \'X\', \'*\' or \'-\' !\n");
				i = wlen + 2;
			}
		}
		if(i >= wlen + 2) continue;
		
		if (i == wlen + 1 && score[wlen] != '\n') {
			while ((score[0] = getchar()) != '\n');
			printf("Too long!\n");
			continue;
		} else if (i < wlen) {
			printf("Too short!\n");
			continue;
		}
		
		for (i = 0; i < wlen; i++) {
			if (score[i] == 'X') {
				if (num[try[i] - 65] > 0) num[try[i] - 65]--;
				word[i] = try[i];
			} else if (score[i] == '*') {
				int k = 0;
				for (; k < wlen; k++) if (word[k] == try[i] && word[k] != try[k]) break;
				if (k == wlen) tmpnum[try[i] - 65]++;
			} else if (tmpnum[try[i] - 65] == 0){
				num[try[i] - 65]--;
				tmpnum[try[i] - 65]--;
			}
		}
		
		for (i = 0; i < 26; i++) if (tmpnum[i] > num[i] && num[i] >= 0) num[i] = tmpnum[i];
		printf("Currently known correct letters: %s\n", word);
		unknown = 0;
		for (i = 0; i < wlen; i++) if (word[i] == '*') unknown++;
		printf("Possible Solutions:");
		if (unknown) {
			int psc = 30;
			int numpossibles = 0;
			int possiblehist[26] = {0};
			int possibles[psc];
			for (i = 0; i < wlistlen; i++) {
				int dictnum[26] = {0};
				int j;
				for (j = 0; j < wlen; j++) {
					char c = wordlist[i][j];
					if (word[j] == '*') {
						if (num[c - 65] < 0) break;
						int allowed = 1;
						for (int k = 0; k <= trycount; k++) if (tries[k][j] == c) allowed = 0;
						if (!allowed) break;
						dictnum[c - 65]++;
					} else if (word[j] != c) break;
				}
				if (j == wlen) {
					int found0 = 1;
					for (int k = 0; k < 26; k++) if (dictnum[k] < num[k]) found0 = 0;
					if (found0) {
						if (numpossibles % 6 == 0) putchar('\n');
						printf("%s\t", wordlist[i]);
						possibles[numpossibles] = i;
						numpossibles++;
						for (j = 0; j < wlen; j++) if (word[j] == '*') possiblehist[wordlist[i][j] - 65]++;
						if (numpossibles >= psc) break;
					}
				}
			}
			if (numpossibles > 2 ) {
				int maxscore = 0;
				char nextsuggest[wlen + 1];
				putchar('\n');
				for (int i = 0, j = 0; i < wlistlen; i++) {
					int locnum[26] = {0};
					int score = 0;
					int forbidknown = (i != possibles[j]);
					
					for (int k = 0; k < wlen; k++) {
						
						int nothere = 0;
						for (int l = 0; l <= trycount; l++) if (tries[l][k] == wordlist[i][k]) nothere = 1;
						if (num[wordlist[i][k] - 65] > 0) nothere *= 5;
						int isknown = (wordlist[i][k] == word[k]);
						score += possiblehist[wordlist[i][k] - 65] * (1 + (word[k] == '*')) / (3 * locnum[wordlist[i][k] - 65] + 1) * (1 - forbidknown * isknown) / (1 + (!isknown) * nothere);
						if (!isknown) locnum[wordlist[i][k] - 65]++;
					}
					if (!forbidknown) {
						score *= 2;
						j++;
					}
					if (score > maxscore) {
						for (int k = 0; k < wlen; k++) nextsuggest[k] = wordlist[i][k];
						nextsuggest[wlen] = '\0';
						maxscore = score;
						for (int k = 0; k < 26; k++) printf("%d ", locnum[k]);
						printf("%s\n", wordlist[i]);
					}
				}
				printf("\nSuggestion for letter discovery: %s\n", nextsuggest);
			}
			//for (int k = 0; k < 26; k++) printf("%d ", possiblehist[k]);
			putchar('\n');
			if (numpossibles == 0) {
				printf("\nNo compatible words found!\nPlease enter solution for addition to word list!\n");
				for (int i = 0; i < wlen; i++) {
					word[i] = getchar();
					if (word[i] > 96 && word[i] < 123) word[i] -= 32;
				}
				printf("%s\n", word);
				//putchar('\n');
				trycount = -1;
				break;
			}
			if (numpossibles == 1) {
				trycount++;
				printf("\nOnly one solution remaining, exiting.\n");
				unknown = 0;
			}
		}
		putchar('\n');
	}
	if (trycount > 0) {
		if (unknown) printf("Out of tries!\n");
		else {
			printf("Found in %d tries!\n", trycount);
		}
	}
	
	int isnewword = 1;
	for (int i = 0; i < wlen; i++) if (word[i] < 65 || word[i] > 90) isnewword = 0;
	if (isnewword) {
		for (int i = 0; i < wlistlen; i++) {
			int j;
			for (j = 0; j < wlen; j++) {
				if (wordlist[i][j] != word[j]) break;
			}
			if (j == wlen) {
				isnewword = 0;
				break;
			}
		}
	}
	if (isnewword) {
		wfile = fopen("wordlist-english0.txt", "a");
		fprintf(wfile, "%s\n", word);
		fclose(wfile);
	}
	return 0;
}
