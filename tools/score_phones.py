#!/usr/bin/python

import subprocess
from editdistance import *

# Define testing parameters
grammar = "grammar/phone.txt"
numbers = ["720-771-0503", "703-965-6152", "724-681-9863", "214-907-6399", \
           "973-368-7462", "703-599-5043", "925-640-8861", "516-314-2363", \
           "719-360-5127", "412-407-7573", "907-360-8667", "571-294-4711", \
           "404-323-0968", "301-518-8448", "902-247-1373", "333-352-6275", \
           "297-753-8295", "735-2856", "260-0926", "236-2347", "642-6234", \
           "234-2374", "847-9753", "832-9265", "695-5702"]

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



    # Prettify the phone number
    if len(resultNumbers) == 10:
        resultString = "".join(resultNumbers[0:3] + ["-"] + resultNumbers[3:6] \
                               + ["-"] + resultNumbers[6:])
    else:
        resultString = "".join(resultNumbers[0:3] + ["-"] + resultNumbers[3:])

    print lastLine
    print
    print "Expect:  "+number
    print "Got:     "+resultString
    print "Dist:    "+str(dist)

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
