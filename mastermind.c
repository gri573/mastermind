#include <stdio.h>
//#include <stdlib.h>

//#define upper(X, ...) _Generic((X), char: upperc(X), char*: uppers(X, __VA_ARGS__))
//#define lower(X, ...)

//Stringhandling
//pending: how to convert ä, ö, ü, ß
char upperc(char c) {
	if (c >= 97 && c < 123) c -= 32;
	return c;
}
char lowerc(char c) {
	if (c > 64 && c < 91) c += 32;
    return c;
}
char* uppers(char* str) {
	while (*str != 0) { 
		*str = upperc(*str);
		str++;
	}
	return str;
}
char* lowers(char* str) {
	while (*str != 0) {
        *str = lowerc(*str);
		str++;
	}
    return str;
}
int max(int a, int b) {
	if (a > b) return a;
	return b;
}
int equal(char* str1, char*str2) {
	int i = 0;
	int same = 1;
	while (str1[i] != 0) {
		if (str1[i] != str2[i]) {
			return 0;
		}
		i++;
	}
	if (str2[i] != 0) same = 0;
	return same;
}

char* printable(char* str, char* prnt) {
	int i = 0;
	int u = 0; //for prnt, with umlaute
	while (str[i] != 0) {
		//for -65 < str[i] < 0 it's interpreted as ? anyway
		if (str[i] < -64) {
			prnt[u++] = -61;
		}
		prnt[u++] = str[i++];
	}
	prnt[u] = 0;
	return prnt;
}

/***How to handle Umlaute: **
	An Umlaut is saved in a String as 2 characters:
	first a -61 to indicate a following Umlaut
	second a negative number which as an unsigned 8-Bit (so +256) corresponds to the characters unicode number, up to #191
	example: ä is represented as {-61, -92}, bc its unicode number is 164

	Here:
	the -61 is discarded when read and the method printable(char* str, char* prnt) adds them again for each negativ char in str
	can use prntbl for second argument
*/


int main(int argc, char* argv[]) {

printf("GUESS THE WORD\n--------------\n\nhow to play:\nAs the title suggests, the goal is to guess a word.\nYou will be given the amount of letters the word contains before your first guess.\nEach letter in your guess will be marked as follows:\nX - The letter is correct and in the correct location.\n* - The letter is correct, but in an incorrect location.\n_ - The letter is incorrect or you have entered it more times than it occurs in the word.\n\nType \"??\" for an in-game help menu\n\n");

//printf("before init\n");
FILE* listfile;
//printf("after init. argc: %d\n", argc);
//Wordlist-File, default english
if (argc > 1) {
    listfile = fopen(argv[1], "r");
} else {
    //printf("!");
	listfile = fopen("wordlist-english.txt", "r");
    printf("no wordlist specified, using default (frequent english)\n");
}

//printf("???\n");

//load wordlist -> How is it later used? Not at all really, beyond getting the word and pot description

int wlistlen = 0;
char c;
//printf("new vars\n");
while ((c = getc(listfile)) != EOF) {
	if (c == '\n') wlistlen++;
}
//printf("before rewind\n");
rewind(listfile);
//printf("after rewind\n");

//pseudo-random, might improve later
int windex = ((long)(&c)/4) % wlistlen;
//printf("random index: %d\n", windex);

//set stream to word
for (int i = 0; i < windex; i++) {
	while (getc(listfile) != '\n');
}
//and remember stream position
const long wordpos = ftell(listfile);
//printf("word position: %d\n", wordpos);

int wordlen = 0;
int desclen = 0;

while ((c = getc(listfile)) != '\n' && c != ';') {
	if (c == -61) c = getc(listfile);
	wordlen++;
}
//printf("word length: %d\n", wordlen);
if (c == ';') {
	while ((c = getc(listfile)) != '\n') {
		desclen++;
	}
}
//printf("description length: %d\n", desclen);
//printf("before SEEK_SET\n");
fseek(listfile, wordpos, SEEK_SET);
//printf("after SEEK_SET\n");

char word0[wordlen + 1];
char desc[desclen + 2];
//printf("declared word and desc\n");

//printf("before saving word\n");
for (int i = 0; i < wordlen; i++) {
	if ((c = getc(listfile)) == -61) c = getc(listfile);
	word0[i] = c;
}
//printf("After saving word\n");
word0[wordlen] = 0;
lowers(word0);
//printf("word: %s\n", word0);

for (int i = 0; i < desclen; i++) {
	desc[i] = getc(listfile);
}
desc[desclen] = '\n'; desc[desclen + 1] = 0;
//printf("done\n");

fclose(listfile);
//printf("file closed\n");

printf("The current word contains %d letters.\n\n", wordlen);

//word0 and printstring0 remain constant for future reference
char word[wordlen+1];
char printstring[wordlen + 1];
char printstring0[wordlen + 1];
//0-Byte to be used as strings

for (int i = 0; i < wordlen; i++) {
    word[i] = word0[i];
    printstring0[i] = '_';
}

printf("%s\n", printstring0);

int tries = 0;
char entered[wordlen + 1];
int alphsize = 26;
char wrongletters[alphsize + 1]; for (int i = 0; i <= alphsize; i++) wrongletters[i] = 0;
char rightletters[wordlen]; for (int i = 0; i < wordlen; i++) rightletters[i] = 0;
int wl_count = 0, rl_count = 0;
//reserve space for making strings printable, 8-Byte safety margin
char prntbl[2*max(alphsize, wordlen) + 8];
int found = 0; //local variable during guessing

entered[wordlen] = word[wordlen] = printstring[wordlen] = printstring0[wordlen] = 0;

//guessing
do {
	int len = 0;
	while (len < wordlen && (entered[len] = getchar()) != '\n') {
		if (entered[len] == -61) {
			entered[len] = getchar();
		}
		len++;
    }
	if (len >= wordlen) while (getchar() != '\n') len++;
	lowers(entered);

	//helpmenu
    if (entered[0] == '?' && entered[1] == '?') {
		//show helpmenu, pot. with description option
        if (desc[0] != '\n') 
			printf("In-Game Help Menu\n\nOptions:\nH - get hint\nS - solve\nD - show description\nX - exit\n\n");
		else 
			printf("In-Game Help Menu\n\nOptions:\nH - get hint\nS - solve\nX - exit\n\n");        

		//await user-prompt
		char prompt = getchar();
		while (getchar() != '\n');
        //case hint
		if (upperc(prompt) == 'H') {
            char letterremains[wordlen]; 
			for (int i = 0; i < wordlen; i++) letterremains[i] = 0;

			int remaining = 0;
			//check all letters in the word
			for (int i = 0; i < wordlen; i++) {
                int found = 0;
				//against letters the player already found
                for (int j = 0; j < wordlen && rightletters[j] != 0; j++) {
                    if (word[i] == rightletters[j])
                        found = 1;
				}
                if (!found) {
                    letterremains[remaining] = word[i];
					remaining++;
				}
			}
            
			if (remaining != 0) {
                int r = ((long)(&r)/4) % remaining; //random in letterremains;
                rightletters[rl_count] = letterremains[r];
				rl_count++;
				//reset: printstring = ""
                int used = 0;
				int inH = 0;
                while (!used) {
                    if (letterremains[r] == word0[inH]) {
                        printstring[inH] = word0[inH];
                        used = 1;
					} else
                        printstring[inH] = '-';
					inH++;
                }
				while (inH < wordlen) {
					printstring[inH] = '-';
					inH++;
				}
				printf("%s\n", printable(printstring, prntbl));
            }
			//if all letters are known, no hint
			else printf("You're almost there, try harder!");
        }
		//case solution
		else if (upperc(prompt) == 'S') {
            printf("solution: %s\n", word0);
            tries = -1;
            break;
		}
        //case Description
		else if (upperc(prompt) == 'D')
            printf("%s\n", desc);

        printf("%s\n", printstring0);
        
		//get new input
		len = 0;
    	while (len < wordlen && (entered[len] = getchar()) != '\n') {
        	if (entered[len] == -61) {
            	entered[len] = getchar();
        	}
       		len++;
    	}
		if (len >= wordlen) while (getchar() != '\n') len++;
    	lowers(entered);
	}
    //end helpmenu. end with new input

	if (len != wordlen) {
        printf("please enter exactly %d letters\n", wordlen);
    	continue;
	}
	else { //'else' obsolete?
        char usedletters[wordlen];
		int ul_count = 0;
		//first check for correct placement to prefer those spots
        for (int i = 0; i < wordlen; i++) {
            if (word[i] == entered[i]) {
                usedletters[ul_count] = entered[i];
				ul_count++;
                found = 0;
                for (int j = 0; j < rl_count; j++) {
                    if (rightletters[j] == entered[i])
                        found = 1;
				}
                if (!found) {
                    rightletters[rl_count] = entered[i];
					rl_count++;
				}
			}
		}
        for (int k = 0; k < wordlen; k++) {
            if (word[k] == entered[k])
                printstring[k] = 'X';
            else {
                found = 0;
                for (int i = 0; i < wordlen; i++) {
                    if (entered[k] == word[i]) 
                        found++;
				}
                if (!found) {
                    for (int j = 0; j < wl_count; j++)
                        if (wrongletters[j] == entered[k])
                            found = 1;
                    if (!found) {
                		wrongletters[wl_count] = entered[k];
						wl_count++;
					}
					found = 0;
				}
				for (int i = 0; i < ul_count; i++)
                    if (entered[k] == usedletters[i])
                        found--;
                if (found > 0) {
                    printstring[k] = '*';
                    usedletters[ul_count] = entered[k];
					ul_count++;
                }
				else {
                    printstring[k] = '_';
                    found = 0; //obsolete?
        		}
			}
		}
		printstring[wordlen] = 0;
        printf("%s - wrongly used letters: %s\n", printstring, wrongletters);
        tries++;
	}
} while (!equal(entered, word0));
//end guessing

uppers(word0);

if (tries >= 0)
    printf("%s found in %d tries\n", printable(word0, prntbl), tries);
if (desc[0] != '\n')
    printf("Description: %s\n", desc);
}
