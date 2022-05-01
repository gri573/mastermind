#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	FILE* instream = stdin;
	FILE* outstream = stdout;
	if (argc >= 2) {
		instream = fopen(argv[1], "r");
		if (instream == NULL) {
			fprintf(stderr, "invalid input file!\n");
			return -1;
		}
		if (argc >= 3) {
			outstream = fopen(argv[2], "w");
			if (outstream == NULL) {
				fprintf(stderr, "invalid output file!\n");
				return -1;
			}
			if (argc > 3) {
				fprintf(stderr, "incorrect flags, usage: cleanwlist [input file] [output file]\n");
				return -1;
			}
		}
	}
	int maxwlen = 64;
	int memstep = 1024;
	int currentmem = memstep;
	char* wlist = (char*) malloc(memstep * maxwlen * sizeof(char));
	for (int i = 0; i >= 0; i++) {
		if (i >= currentmem) {
			currentmem += memstep;
			wlist = (char*) realloc((void*) wlist, currentmem * maxwlen * sizeof(char));
		}
		int j = 0;
		char lc = 0;
		int samecount = 0;
		int chars[256] = {0};
		while(1) {
			char c = fgetc(instream);
			if (c == EOF) {
				i = -10;
				j = -1;
				//break;
				fclose(instream);
				fclose(outstream);
				return 0;
			}
			if (c == '\n') {
				*(wlist + maxwlen * i + j) = '\0';
				break;
			}
			if (c > 96 && c < 123) c -= 32;
			if (c == lc) samecount++;
			else samecount = 0;
			lc = c;
			chars[c]++;
			if (c < 65 || c > 90 || j >= maxwlen || samecount > 1) {
				j = -1;
				while ((c = fgetc(instream)) != '\n' && c != EOF);
				if (c == EOF) {
					i = -10;
					j = -1;
					//break;
					fclose(instream);
					fclose(outstream);
					return 0;
				}
				break;
			}
			*(wlist + maxwlen * i + j) = c;
			j++;
		}
/*		if (j >= 0) {
			int lettercount = 0;
			int singleletters = 0;
			for (int l = 0; l < 256; l++) {
				if (chars[l]) {
					if (chars[l] == 1) singleletters++;
					lettercount++;
				}
			}
			if (j > 3 && lettercount < 3) j = -1;
			if (singleletters * 2 < j) j = -1;
		}
		if (j >= 0) {
			int new = 1;
			for (int l = 0; l < i; l++) {
				int same = 1;
				for (int k = 0; k < maxwlen; k++) {
					if (*(wlist + maxwlen * i + k) != *(wlist + maxwlen * l + k)) {
						same = 0;
						break;
					}
					if (*(wlist + maxwlen * i + k) == '\0') {
						break;
					}
				}
				if (same) new = 0;
			}
			if (!new) {
				j = -1;
				fprintf(stderr, ".");
			}
		}*/
		if (j >= 0) {
			if (!((i / 1024 * i / 1024) % 10 || i % 1024)) fprintf(stderr, "%d\n", i);
			fprintf(outstream, "%s\n", wlist + maxwlen * i);
		} else {
			i--;
		}
	}
	free((void*) wlist);
	return 0;
}
