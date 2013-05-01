#!/usr/bin/python

import subprocess

# Define testing parameters
transcript = open("../hwdata/TEST.transcripts", "r");


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
totalDistance = 0
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
    resultString = subprocess.check_output(["bin/scorephonemeaurora",test])

    # Process the result into a list of numbers
    # Chop off the first result - it just says "Result:"
    lastLine = resultString.split("\n")[-2]
    words = filter(lambda w: w != "sil", lastLine.split(" ")[1:])


    # Compare result to template
    totalNumbers += 1
    totalDigits += len(expected)

    distance = editDistance(expected, words)
    totalDistance += distance
    extraDigits += max(len(words)-len(expected), 0)

    if distance == 0:
        totalCorrectNumbers += 1


    print lastLine
    print
    print "Expect:  " + str(expected)
    print "Got:     " + str(words)
    print "Dist:    " + str(distance)

# Return final results
print
print
print "FINAL RESULTS:"
print "    " + str(totalDistance) + " edit distance from " + str(totalDigits) + \
      " digits total"
print "    " + str(extraDigits) + " extra digits detected"
print "    " + str(totalCorrectNumbers) + "/" + str(totalNumbers) + \
      " numbers correct"
