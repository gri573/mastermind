#include <stdio.h>
#include <stdlib.h>
int printword(signed char word[]) {
	for (int i = 0; 1; i++) {
		if (word[i] == 0) return 0;
		if (word[i] == '*' || (word[i] > 64 && word[i] < 91)) putchar(word[i]);
		else switch (word[i]) {
			case 91: printf("Á"); break;
			case 92: printf("É"); break;
			case 93: printf("Í"); break;
			case 94: printf("Ó"); break;
			case 95: printf("Ú"); break;
			case 96: printf("Ä"); break;
			case 97: printf("Ö"); break;
			case 98: printf("Ü"); break;
			case 99: printf("ß"); break;
			case 100: printf("Ő"); break;
			case 101: printf("Ű"); break;
			default: {
				fprintf(stderr, "Bad characters in known letter string: %s\n", word);
				return -1;
			}
		}
	}
}

typedef struct wordlist_entry {
	int score;
	int id;
	struct wordlist_entry* next;
	struct wordlist_entry* prev;
} wentry;

int main(int argc, const char* argv[]) {
	// read word list
	int wlistlen = 14000;
	int wlen = -1;
	if (argc >= 2) {
		wlen = 0;
		signed char c;
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
	signed char wordlist[wlistlen][wlen + 1];
	FILE* wfile;
	if (argc <= 2) wfile = fopen("en_1.txt", "r");
	else wfile = fopen(argv[2], "r");
	if (wfile == NULL) {
		fprintf(stderr, "Wordlist file missing!\n");
		return -1;
	}
	for (int i = 0; i < wlistlen; i++) {
		signed char c; 
		int k;
		for (k = 0; (c = fgetc(wfile)) != '\n'; k++) {
			if (c == EOF) {
				wlistlen = i - 2;
				break;
			}
			if (k < wlen) {
				if (c < 65 || c > 101) k = wlen + 1;
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
	int num[37] = {0};
	signed char word[wlen + 1];
	signed char tries[wlen * 3 / 2 + 5][wlen + 1];
	for(int i = 0; i < wlen; i++) word[i] = '*';
	word[wlen] = '\0';
	printf("please enter your guess, followed by its score, separated by a line break:\n");
	for (trycount = 0; trycount < wlen * 3 / 2 + 5 && unknown; trycount++) {
		printf("new try\n");
		int tmpnum[37] = {0};
		signed char try[wlen + 1];
		signed char score[wlen + 1];
		int i;
		for (i = 0; i <= wlen && (try[i] = getchar()) != '\n'; i++) {
			if (try[i] > 96 && try[i] < 123) try[i] -= 32;
			if (try[i] == -61) {
				signed char c = getchar();
				switch (c) {
					case -127: try[i] = 91; break; // Á
					case -95: try[i] = 91; break;
					case -119: try[i] = 92; break; // É
					case -87: try[i] = 92; break;
					case -115: try[i] = 93; break; // Í
					case -83: try[i] = 93; break;
					case -109: try[i] = 94; break;// Ó
					case -77: try[i] = 94; break;
					case -102: try[i] = 95; break;// Ú
					case -70: try[i] = 95; break;
					case -124: try[i] = 96; break;// Ä
					case -92: try[i] = 96; break;
					case -106: try[i] = 97; break;// Ö
					case -74: try[i] = 97; break;
					case -100: try[i] = 98; break;// Ü
					case -68: try[i] = 98; break;
					case -97: try[i] = 99; break; // ß
					default: fprintf(stderr, "Invalid letter!\n");
				}
			} else if (try[i] == -59) {
				signed char c = getchar();
				if (c == -112 || c == -111) try[i] = 100;    // Ő
				else if (c == -80 || c == -79) try[i] = 101; // Ű
				else fprintf(stderr, "Invalid letter!\n");
			}
		}
		if (i == wlen + 1 && try[wlen] != '\n') {
			while ((score[0] = getchar()) != '\n');
			fprintf(stderr, "Too long!\n");
			trycount--;
			continue;
		} else if (i < wlen) {
			fprintf(stderr, "Too short!\n");
			trycount--;
			continue;
		}
		for (i = 0; i < wlen; i++) tries[trycount][i] = try[i];
		tries[trycount][wlen] = '\0';
		for (i = 0; i <= wlen && (score[i] = getchar()) != '\n'; i++) {
			if (score[i] == 'x') score[i] = 'X';
			else if (score[i] == '_'
			      || score[i] == '.') score[i] = '-';
			else if (score[i] == '+') score[i] = '*';
			else if (score[i] != 'X' && score[i] != '*' && score[i] != '-') {
				fprintf(stderr, "Invalid scoring caracter, use \'X\', \'*\' or \'-\' !\n");
				i = wlen + 2;
			}
		}
		if(i >= wlen + 2) continue;
		
		if (i == wlen + 1 && score[wlen] != '\n') {
			while ((score[0] = getchar()) != '\n');
			fprintf(stderr, "Too long!\n");
			continue;
		} else if (i < wlen) {
			fprintf(stderr, "Too short!\n");
			continue;
		}
		
		for (i = 0; i < wlen; i++) {
			if (score[i] == 'X') {
				if (num[try[i] - 65] > 0) num[try[i] - 65]--;
				word[i] = try[i];
			} else if (score[i] == '*') {
				int k = 0;
				for (; k < wlen; k++) if (word[k] == try[i] && word[k] != try[k]) {
					k = -5;
					break;
				}
				if (k >= 0) {
					tmpnum[try[i] - 65]++;
				}
			} else if (tmpnum[try[i] - 65] == 0){
				num[try[i] - 65]--;
				tmpnum[try[i] - 65]--;
			}
		}


		for (i = 0; i < 37; i++) if (tmpnum[i] > num[i] && num[i] >= 0) num[i] = tmpnum[i];

		unknown = 0;
		for (i = 0; i < wlen; i++) if (word[i] == '*') unknown++;
		if (!unknown) for (i = 0; i < wlen; i++) if (word[i] != try[i]) unknown = 1;
		if (unknown) {
			printf("Possible Solutions:");
			int psc = 600;
			int numpossibles = 0;
			int possiblehist[37] = {0};
			int possibles[psc];
			signed char word0[wlen + 1];
			for (i = 0; i <= wlen; i++) word0[i] = 0;

			for (i = 0; i < wlistlen; i++) {
				int dictnum[37] = {0};
				int j;
				for (j = 0; j < wlen; j++) {
					signed char c = wordlist[i][j];
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
					for (int k = 0; k < 37; k++) if (dictnum[k] < num[k]) found0 = 0;
					if (found0) {
						if (numpossibles < 30) {
							if (numpossibles % 6 == 0) putchar('\n');
							printword(wordlist[i]);
							putchar('\t');
						}
						for (j = 0; j < wlen; j++) {
							if (word0[j] == 0) word0[j] = wordlist[i][j];
							else if (word0[j] != wordlist[i][j]) word0[j] = '*';
						}
						possibles[numpossibles] = i;
						numpossibles++;
						//for (j = 0; j < wlen; j++) if (word[j] == '*') possiblehist[wordlist[i][j] - 65]++;
						if (numpossibles >= psc) break;
					}
				}
			}
			if (numpossibles > 1) for (i = 0; i < wlen; i++) {
				if (word[i] == '*' && word0[i] != '*') {
					if (num[word0[i] - 65] > 0) num[word0[i] - 65]--;
					word[i] = word0[i];
				}
			}
			printf("\nCurrently known correct letters: ");
			printword(word0);
			if (numpossibles > 2 ) {
				for (int i = 0; i < numpossibles; i++) {
					for (int j = 0; j < wlen; j++) if (word[j] == '*') possiblehist[wordlist[possibles[i]][j] - 65]++;
				}
				int unknownc = 0;
				for (int i = 0; i < wlen; i++) if (word[i] == '*') unknownc++;
				putchar('\n');
				int suggestcount = 30;
				void* bestlistentrypoint = calloc(suggestcount, sizeof(wentry));
				wentry* bestlist = (wentry*) bestlistentrypoint;
				for (int i = 0, j = 0; i < wlistlen; i++) {
					int locnum[37] = {0};
					int score = 0;
					int forbidknown = (i != possibles[j]);
					
					for (int k = 0; k < wlen; k++) {
						
						int nothere = 0;
						for (int l = 0; l <= trycount; l++) if (tries[l][k] == wordlist[i][k]) nothere = 1;
						if (num[wordlist[i][k] - 65] > 0) nothere *= 5;
						int isknown = (wordlist[i][k] == word[k]);
						score += possiblehist[wordlist[i][k] - 65] * (1 + (word[k] == '*')) / (3 * locnum[wordlist[i][k] - 65] + 1) * (1 - forbidknown * 0 - isknown) / (1 + (!isknown) * nothere);
						if (!isknown) locnum[wordlist[i][k] - 65]++;
					}
					if (!forbidknown) {
						score = (int) (score * (1.0 + 2.0 * unknownc / numpossibles));
						j++;
					}
					wentry* currententry = bestlist;
					for (int l = 0; l < suggestcount; l++) {
						if (currententry->score < score) {
							wentry* lastentry = currententry;
							wentry* newentry;
							l++;
							for (; lastentry->next != NULL && l < suggestcount; l++) {
								lastentry = lastentry->next;
							}
							if (l < suggestcount) {
								newentry = ((wentry*) bestlistentrypoint) + l;
							} else {
								newentry = lastentry;
							}
							if (newentry != currententry) newentry->next = currententry;
							newentry->id = i;
							newentry->score = score;
							newentry->prev = currententry->prev;
							if (newentry->prev != NULL) newentry->prev->next = newentry;
							else bestlist = newentry;
							currententry->prev = newentry;
							break;
						}
						if (currententry->next != NULL) currententry = currententry->next;
					}
				}
				printf("\n\nSuggestions for letter discovery (best first):\n");
				wentry* currententry = bestlist;
				for (int i = 0; i < suggestcount && currententry != NULL; i++) {
					if (i) printf(", ");
					if (i != 0 && i % 6 == 0) putchar('\n');
					printword(wordlist[currententry->id]);
					currententry = currententry->next;
				}
				free(bestlistentrypoint);
				//printword(nextsuggest);
				putchar('\n');
			}
			//for (int k = 0; k < 37; k++) printf("%d ", possiblehist[k]);
			putchar('\n');
			if (numpossibles == 0) {
				printf("\nNo compatible words found!\nPlease enter solution for addition to word list!\n");
				for (int i = 0; i < wlen; i++) {
					word[i] = getchar();
					if (word[i] > 96 && word[i] < 123) word[i] -= 32;
					if (try[i] == -61) {
						signed char c = getchar();
						switch (c) {
							case -127: try[i] = 91; break; // Á
							case -95: try[i] = 91; break;
							case -119: try[i] = 92; break; // É
							case -87: try[i] = 92; break;
							case -115: try[i] = 93; break; // Í
							case -83: try[i] = 93; break;
							case -109: try[i] = 94; break;// Ó
							case -77: try[i] = 94; break;
							case -102: try[i] = 95; break;// Ú
							case -70: try[i] = 95; break;
							case -124: try[i] = 96; break;// Ä
							case -92: try[i] = 96; break;
							case -106: try[i] = 97; break;// Ö
							case -74: try[i] = 97; break;
							case -100: try[i] = 98; break;// Ü
							case -68: try[i] = 98; break;
							case -97: try[i] = 99; break; // ß
							default: fprintf(stderr, "Invalid letter!\n");
						}
					} else if (try[i] == -59) {
						signed char c = getchar();
						if (c == -112 || c == -111) try[i] = 100;    // Ő
						else if (c == -80 || c == -79) try[i] = 101; // Ű
						else fprintf(stderr, "Invalid letter!\n");
					}
				}
				printword(word);
				putchar('\n');
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
			printword(word);
			printf(" found in %d tries!\n", trycount);
			int tryhist[10] = {0};
			int ftrycount = 0;
			FILE* histfile = fopen("tryhist.txt", "r");
			if (histfile != NULL) {
				for (int i = 0; i < 10 && fscanf(histfile, "%d ", tryhist + i) == 1; i++);
				fclose(histfile);
			}
			tryhist[trycount]++;
			histfile = fopen("tryhist.txt", "w");
			if (histfile != NULL) {
				for (int i = 0; i < 10; i++) {
					if (tryhist[i] > ftrycount) ftrycount = tryhist[i];
					fprintf(histfile, "%d ", tryhist[i]);
				}
				fclose(histfile);
				printf("Show histogram? (y/N)");
				signed char c;
				while ((c = getchar()) != 'y' && c != 'Y' && c != 'n' && c != 'N' && c != '\n') {
					printf("\nplease enter \"Y\" or \"N\" and confirm with ENTER\n");
					getchar();
				}
				if (c == 'y' || c == 'Y') {
					for (int i = ftrycount; i > 0; i -= ftrycount / 10 + 1) {
						for (int j = 0; j < 10; j++) {
							switch (i > tryhist[j]) {
								case 1: putchar(' '); break;
								case 0: putchar('#'); break;
							}
						}
						putchar('\n');
					}
				} else if (c == 'n' || c == 'N') getchar();
			} else fprintf(stderr, "Cannot evaluate try count histogram (missing file)!\n");
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
		printf("Added %s to wordlist!\n", word);
		fclose(wfile);
	}
	return 0;
}
