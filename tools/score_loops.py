#!/usr/bin/python

import subprocess

# Define testing parameters
grammar = "grammar/loop.txt"
numbers = ["991385", "826414052002", "8212176342", "7343332190377", "2212", \
           "123456", "6890372344", "72184347924", "55555", "37274921"]

# Convienience
def wordToNumber(w):
    wordDict = {"zero":"0", "one":"1", "two":"2", "three":"3", "four":"4", \
                "five":"5", "six":"6", "seven":"7", "eight":"8", "nine":"9"}
    return wordDict[w]

# Not quite edit distance - doesn't punish insertions
def editDistance(w1,w2):
    lastCol = range(0,len(w2))
    if w1[0] != w2[0]:
        lastCol = map(lambda x: x+1, lastCol)

    nextCol = [0]*len(w2)
    for i in xrange(1,len(w1)):
        for j in xrange(0,len(w2)):
            if j == 0:
                nextCol[j] = lastCol[j] + 1
            else:
                left = lastCol[j] + 1
                down = nextCol[j-1]
                downleft = lastCol[j-1]
                if w1[i] != w2[j]:
                    downleft += 1
                nextCol[j] = min(left,down,downleft)
        lastCol = nextCol
        nextCol = [0]*len(w2)

    return lastCol[-1]



extraDigits = 0
totalCorrectDigits = 0
totalDigits = 0
totalCorrectNumbers = 0
totalNumbers = 0
# Run the test against each test number
for number in numbers:
    print
    print    
    print "Testing..."
    
    # Call viterbi in the shell
    recording = "analysis/" + number + "-40.out"
    resultString = subprocess.check_output(["bin/viterbi",grammar,recording])

    # Process the result into a list of numbers
    # Chop off the first result - it just says "Result:"
    lastLine = resultString.split("\n")[-2]
    words = filter(lambda w: w != "sil", lastLine.split(" ")[1:])
    resultNumbers = map(wordToNumber, words);
    

    # Compare result to template
    totalNumbers += 1
    totalDigits += len(number)

    testNumbers = filter(lambda c: c != "-", list(number))
    distance = editDistance(testNumbers, resultNumbers)

    totalCorrectDigits += len(number) - distance
    if distance == 0:
        totalCorrectNumbers += 1

    extraDigits += max(len(resultNumbers)-len(testNumbers), 0)


    print lastLine
    print
    print "Expect:  " + number
    print "Got:     " + "".join(resultNumbers)
    print "Dist:    " + str(distance)

# Return final results
print
print
print "FINAL RESULTS:"
print "    " + str(totalCorrectDigits) + "/" + str(totalDigits) + \
      " digits correct"
print "    " + str(extraDigits) + " extras"
print "    " + str(totalCorrectNumbers) + "/" + str(totalNumbers) + \
      " numbers correct"
