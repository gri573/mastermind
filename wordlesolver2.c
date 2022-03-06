#include <stdio.h>
int main() {
	// read word list
	int wlistlen = 14000;
	printf("Please enter word length: ");
	int wlen;
	scanf("%d", &wlen);
	getchar();
	putchar('\n');
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
		else {
			int new = 1;
			for (int j = 0; j < i && new; j++) {
				// k is repurposed here
				int same = 1;
				for (k = 0; k < wlen && wordlist[j][k] != '\n'; k++) if (wordlist[j][k] != wordlist[i][k]) {
					same = 0;
					break;
				}
				if (same) new = 0;
			}
			if (new) wordlist[i][wlen] = '\0';
			else i--;
		}
	}
	if (wlistlen != 14000) wlistlen += 3;
	printf("Number of available words: %d\n", wlistlen);
	
	int found = 0;
	int trycount;
	int num[26] = {0};
	char word[wlen + 1];
	char tries[wlen * 3 / 2 + 5][wlen + 1];
	for(int i = 0; i < wlen; i++) word[i] = '*';
	word[wlen] = '\0';
	printf("please enter your guess, followed by its score, separated by a line break:\n");
	for (trycount = 0; wlen * 3 / 2 + 5 && !found; trycount++) {
		printf("new try\n");
		int tmpnum[26] = {0};
		char try[wlen + 1];
		char score[wlen + 1];
		int i;
		for (i = 0; i <= wlen && (try[i] = getchar()) != '\n'; i++) if (try[i] > 96 && try[i] < 123) try[i] -= 32;
		if (i == wlen + 1 && try[wlen] != '\n') {
			while ((score[0] = getchar()) != '\n');
			printf("Too long!\n");
			continue;
		} else if (i < wlen) {
			printf("Too short!\n");
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
				tmpnum[try[i] - 65]++;
			} else if (tmpnum[try[i] - 65] == 0){
				num[try[i] - 65]--;
				tmpnum[try[i] - 65]--;
			}
		}
		
		for (i = 0; i < 26; i++) if (tmpnum[i] > num[i] && num[i] >= 0) num[i] = tmpnum[i];
		printf("Currently known correct letters: %s\n", word);
		found = 1;
		for (i = 0; i < wlen; i++) if (word[i] == '*') found = 0;
		if (!found) {
			int numpossibles = 0;
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
						printf("%s\t", wordlist[i]);
						numpossibles++;
						if (numpossibles % 6 == 0) putchar('\n');
						if (numpossibles >= 29) break;
					}
				}
			}
			if (numpossibles == 0) {
				printf("No compatible words found!\n");
				for (int k = 0; k < 26; k++) printf("%d ", num[k]);
				break;
			}
		}
		putchar('\n');
	}
	printf("Found in %d tries!\n", trycount);
	return 0;
}
