from numpy import loadtxt
wlist = loadtxt("wordlist-english0.txt", unpack=True, delimiter=";", dtype=str)
unusedletters = input("available letters: ")
word = input("known letters at correct locations (* for unknown letter): ")
for a in wlist:
    if len(a) == len(word):
        k = 0
        found = True
        for letter in a:
            try:
                if word[k] == "*":
                    found0 = False
                    for letter0 in unusedletters:
                        if letter.upper() == letter0.upper():
                            found0 = True
                    if found0 == False:
                        found = false
                else:
                    if letter.upper() != word[k].upper():
                        found = False
            except:
                found = False
            k += 1
        if found == True:
            print(a)