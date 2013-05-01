#!/usr/bin/python

import subprocess
from editdistance import *

# Define testing parameters
transcript = open("../hwdata/TEST.transcripts", "r");

totalDistance = 0
totalSubs = 0
totalIns = 0
totalDels = 0
totalDigits = 0
totalCorrectNumbers = 0
totalNumbers = 0

# Run the test against each test number
for line in transcript.readlines():
    expected = line.split(" ")[0:-1]
    test = (line.split(" ")[-1])[1:-2]

    print
    print 
    print "Testing",test,"..."
    
    # Call viterbi in the shell
    resultString = subprocess.check_output(["bin/scoreaurora",test])

    # Process the result into a list of numbers
    # Chop off the first result - it just says "Result:"
    lastLine = resultString.split("\n")[-2]
    words = filter(lambda w: w != "sil", lastLine.split(" ")[1:])


    # Compare result to template
    dist, subs, ins, dels = editDistance(words,expected)

    totalDistance += dist
    totalSubs += subs
    totalIns += ins
    totalDels += dels

    totalDigits += len(expected)
    totalNumbers += 1
    if dist == 0:
        totalCorrectNumbers += 1


    print lastLine
    print
    print "Expect:  " + str(expected)
    print "Got:     " + str(words)
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
