import numpy as np
from sys import argv
print("GUESS THE WORD\n--------------\n\nhow to play:\nAs the title suggests, the goal is to guess a word.\nYou will be given the amount of letters the word contains before your first guess.\nEach letter in your guess will be marked as follows:\nX - The letter is correct and in the correct location.\n* - The letter is correct, but in an incorrect location.\n_ - The letter is incorrect or you have entered it more times than it occurs in the word.\n\nType \"??\" for an in-game help menu\n")
try:
    listfname = argv[1]
except:
    listfname = "wordlist-english.txt"
    print("no wordlist specified, using default (frequent english)")
wlist = np.loadtxt(listfname, unpack=True, usecols=0, dtype=str, delimiter=";")
windex = np.random.randint(0, len(wlist))
desc = None
try:
    desclist = np.loadtxt(listfname, unpack=True, usecols=1, dtype=str, delimiter=";")
    desc0 = desclist[windex]
    last = ""
    desc = ""
    for letter in desc0:
        if letter == "n" and last == "\\":
            desc += "\n"
        else:
            if letter != "\\":
                desc += letter
        last = letter
except:
    True
word0 = wlist[windex]
word0 = word0.lower()
print("The current word contains " + str(len(word0)) + " letters.\n")
word = []
printstring = ""
for letter in word0:
    word.append(letter)
    printstring += "_"
printstring0 = printstring
print(printstring)
tries = 0
entered = ""
wrongletters = ""
rightletters = ""
while entered != word0:
    entered = input("")
    entered = entered.lower()
    if entered == "??":
        hintdesc = ""
        if desc != None:
            hintdesc = "D - show description\n"
        print("In-Game Help Menu\n\nOptions:\nH - get hint\nS - solve\n" + hintdesc + "X - exit\n")
        prompt = input("")
        if prompt.upper() == "H":
            letterremains = []
            for letter in word:
                found = 0
                for letter0 in rightletters:
                    if letter == letter0:
                        found = 1
                if found == 0:
                    letterremains.append(letter)
            if letterremains != []:
                l = np.random.randint(0, len(letterremains))
                printstring = ""
                used = 0
                for letter in word:
                    if letterremains[l] == letter and used == 0:
                        printstring += letter
                        used = 1
                    else:
                        printstring += "-"
                print(printstring)
            else:
                print("You're almost there, try harder!")
        if prompt.upper() == "S":
            print("solution: " + word0)
            tries = -1
            break
        if prompt.upper() == "D":
            print(desc)
        print(printstring0)
        entered = input("")
        entered = entered.lower()
    if len(entered) != len(word):
        print("please enter exactly " + str(len(word)) + " letters")
    else:
        k = 0
        printstring = ""
        usedletters = []
        l = 0
        for letter in entered:
            if word[l] == letter:
                usedletters.append(letter)
                found = 0
                for letter0 in rightletters:
                    if letter0 == letter:
                        found = 1
                if found == 0:
                    rightletters += letter
            l += 1
        for letter in entered:
            if k <= len(word):
                if word[k] == letter:
                    printstring += "X"
                else:
                    found = 0
                    for letter0 in word:
                        if letter == letter0:
                            found += 1
                    if found == 0:
                        for letter0 in wrongletters:
                            if letter0 == letter:
                                found = 1
                        if found == 0:
                            wrongletters += letter
                        found = 0
                    for letter0 in usedletters:
                        if letter == letter0:
                            found -= 1
                    if found > 0:
                        printstring += "*"
                        usedletters.append(letter)
                    else:    
                        printstring += "_"
                        found = 0
            else:
                print("unknown error, expect undefined behaviour")
            k += 1
        printstring += " - wrongly used letters: " + wrongletters
        print(printstring)
        tries += 1
if tries >= 0:
    print(word0.upper() + " found in " + str(tries) + " tries\n")
if desc != None:
    print("Description:\n" + desc)