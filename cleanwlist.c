#include <stdio.h>

int main() {
	int maxwlen = 50;
	char wlist[100000][maxwlen];
	for (int i = 0; i < 100000; i++) {
		int j = 0;
		while(1) {
			char c;
			while ((c = getchar()) == ' ');
			if (c == EOF) {
				i = 100001;
				j = -1;
				break;
			}
			if (c == '\n') {
				wlist[i][j] = '\0';
				break;
			}
			if (c > 96 && c < 123) c -= 32;
			if (c < 65 || c > 90 || j >= maxwlen) {
				i--;
				j = -1;
				while (getchar() != '\n');
				break;
			}
			wlist[i][j] = c;
			j++;
		}
		if (j >= 0) {
			int new = 1;
			for (int j = 0; j < i; j++) {
				int same = 1;
				for (int k = 0; k < maxwlen; k++) {
					if (wlist[i][k] != wlist[j][k]) {
						same = 0;
						break;
					}
					if (wlist[i][k] == '\0') {
						break;
					}
				}
				if (same == 1) new = 0;
			}
			if (new) {
				printf("%s\n", wlist[i]);
			} else i--;
		}
	}
	return 0;
}
