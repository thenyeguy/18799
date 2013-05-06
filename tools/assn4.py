#!/usr/bin/python

import re
import string
from editdistance import *


def score_part1():
    # Read in the correct story, and process it to leave only lowercase
    correctFile = open("text/story-correct.txt","r");
    correct = correctFile.read()
    correct = correct.translate(string.maketrans("",""), string.punctuation)
    correctWords = map(lambda s: s.lower(), filter(lambda s: s != '', \
                        correct.replace('\n',' ').split(' ')))

    # Read in test file
    testFile = open("text/story-assn4.out")
    test = " ".join(map(lambda s: s.rstrip('\n'), testFile.readlines()))
    testWords = filter(lambda s: s != '', test.split(' '))
    
    print "Assignment 4 Part 1"
    print "Comparing mispelled against correct story..."

    distance, subs, ins, dels = editDistance(testWords,correctWords)
    print "    Total input length: " + str(len(correctWords))
    print "    Edit-distance: " + str(distance)
    print "    Substitutions: " + str(subs)
    print "    Insertions: " + str(ins)
    print "    Deletions: " + str(dels)
    print


def score_part2a():
    # Read in the correct story, and process it
    correctFile = open("text/story-assn4-correct.txt")
    correctWords = map(lambda s: s.lower(), filter(lambda s: s != '', \
                        correctFile.read().replace('\n',' ').split(' ')))

    # Read in test file
    testFile = open("text/unsegmented-assn4.out")
    test = " ".join(map(lambda s: s.rstrip('\n'), testFile.readlines()))
    testWords = filter(lambda s: s != '', test.split(' '))
    
    print "Assignment 4 Part 2A"
    print "Comparing unsegmented text against correct story..."

    distance, subs, ins, dels = editDistance(testWords,correctWords)
    print "    Total input length: " + str(len(correctWords))
    print "    Edit-distance: " + str(distance)
    print "    Substitutions: " + str(subs)
    print "    Insertions: " + str(ins)
    print "    Deletions: " + str(dels)
    print


def score_part2b():
    # Read in the correct story, and process it
    correctFile = open("text/story-assn4-correct.txt")
    correctWords = map(lambda s: s.lower(), filter(lambda s: s != '', \
                        correctFile.read().replace('\n',' ').split(' ')))

    # Read in test file
    testFile = open("text/unsegmented-incorrect-assn4.out")
    test = " ".join(map(lambda s: s.rstrip('\n'), testFile.readlines()))
    testWords = filter(lambda s: s != '', test.split(' '))
    
    print "Assignment 4 Part 2B"
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

    score_part1()
    score_part2a()
    score_part2b()

if __name__ == "__main__":
    main()
