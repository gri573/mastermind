from numpy import loadtxt
wlist = loadtxt("wordlist-english0.txt", unpack=True, delimiter=";", dtype=str)
unusedletters = input("available letters: ")
word = input("known letters at correct locations (* for unknown letter): ")
foundwords = []
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
            for prev in foundwords:
                if prev == a.upper():
                    found = False
            if found == True:
                foundwords.append(a.upper())
                print(a)