#!/usr/bin/python

import re
import string
from editdistance import *


def score():
    # Read in the correct story, and process it
    correctFile = open("text/prepped-correct-original.txt")
    correctWords = filter(lambda s: s != '', \
                    correctFile.read().replace('\n',' ').split(' '))

    # Read in test file
    testFile = open("text/prepped-story-original.out")
    test = " ".join(map(lambda s: s.rstrip('\n'), testFile.readlines()))
    testWords = filter(lambda s: s != '', test.split(' '))
    
    print "Assignment 2"
    print "Comparing unsegmented/mispelled text against correct story..."

    distance, subs, ins, dels = editDistance(testWords,correctWords)
    print "    Total input length: " + str(len(correctWords))
    print "    Edit-distance: " + str(distance)
    print "    Substitutions: " + str(subs)
    print "    Insertions: " + str(ins)
    print "    Deletions: " + str(dels)
    print


def main():
    print

    score()

if __name__ == "__main__":
    main()
