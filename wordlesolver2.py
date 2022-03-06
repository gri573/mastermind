from numpy import loadtxt;
wlist = loadtxt("wordlist-english0.txt", unpack=True, delimiter=";", dtype=str)
found = False
letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
availableletters = [1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]
requiredletters =  [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
guesses = []
corrects = []
word = "*****"
while not found:
    guess = input("please enter your guess and its grading (x fully correct; * wrong place; - not in word):\n")
    guesses.append(guess.upper())
    correct0 = input("")
    correct = []
    newrequiredletters = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
    for letter in correct0:
        if letter.upper() == "X":
            correct.append(2)
        else:
            if letter.upper() == "*":
                correct.append(1)
            else:
                if letter.upper() == "-" or letter.upper() == "_":
                    correct.append(0)
                else: raise Exception("Invalid correctness specifier!")
    for l in range(len(guess)):
        if correct[l] == 2:
            word[l] = guess[l]
            for k in range(len(letters)):
                if guess[l] == letters[k]:
                    requiredletters[k] = max(requiredletters[k] - 1, 0)
        else:
            for k in range(len(letters)):
                if letters[k] == guess[l]:
                    if correct == 1:
                        newrequiredletters[k] += 1
                        requiredletters[k] = max(newrequiredletters[k], requiredletters[k])