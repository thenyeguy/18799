#!/usr/bin/python

import subprocess
from editdistance import *

# Define testing parameters
grammar = "grammar/loop.txt"
numbers = ["991385", "826414052002", "8212176342", "7343332190377", "2212", \
           "123456", "6890372344", "72184347924", "55555", "37274921"]

# Convienience
def wordToNumber(w):
    wordDict = {"zero":"0", "one":"1", "two":"2", "three":"3", "four":"4", \
                "five":"5", "six":"6", "seven":"7", "eight":"8", "nine":"9"}
    return wordDict[w]



totalDistance = 0
totalSubs = 0
totalIns = 0
totalDels = 0
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
    expectedResult = filter(lambda c: c != "-", list(number))
    dist, subs, ins, dels = editDistance(resultNumbers, expectedResult)

    totalDistance += dist
    totalSubs += subs
    totalIns += ins
    totalDels += dels

    totalDigits += len(expectedResult)
    totalNumbers += 1
    if dist == 0:
        totalCorrectNumbers += 1

    print lastLine
    print
    print "Expect:  " + number
    print "Got:     " + "".join(resultNumbers)
    print "Dist:    " + str(dist)


# Return final results
print
print
print "FINAL RESULTS:"
print "    From " + str(totalDigits) + " total digits..."
print "        Distance: " + str(totalDistance)
print "        Substitutions: " + str(totalSubs)
print "        Insertions: " + str(totalIns)
print "        Deletions: " + str(totalDels)
print "    " + str(totalCorrectNumbers) + "/" + str(totalNumbers) + \
      " numbers correct"
