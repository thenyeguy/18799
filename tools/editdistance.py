def editDistance(s1, s2):
    """ Returns (distance,substitutions,insertions,deletions """

    # Null pad the sequences
    s1 = [None] + s1
    s2 = [None] + s2

    # Initialize lastcol
    lastCol = zip(range(0,len(s2)), [0]*len(s2), [0]*len(s2), range(0,len(s2)))
    # Initialize nextcol
    nextCol = [(0,0,0,0)] * len(s2)

    # Score all cols
    for col in xrange(1,len(s1)):
        # Score all rows
        # Bottom first
        score, subs, ins, dels = lastCol[0]
        nextCol[0] = (score+1, subs, ins+1, dels)

        # The rest
        for row in xrange(1,len(s2)):
            # Downleft
            score1, subs1, ins1, dels1 = lastCol[row-1]
            if s1[col] != s2[row]:
                score1 += 1
                subs1 += 1
                
            # Left
            score2, subs2, ins2, dels2 = lastCol[row]
            score2 += 1
            ins2 += 1
            
            # Down
            score3, subs3, ins3, dels3 = nextCol[row-1]
            score3 += 1
            dels3 += 1

            if score1 < score2 and score1 < score3:
                nextCol[row] = (score1, subs1, ins1, dels1)
            elif score2 < score3:
                nextCol[row] = (score2, subs2, ins2, dels2)
            else:   
                nextCol[row] = (score3, subs3, ins3, dels3)

        # Prep next col
        lastCol = nextCol
        nextCol = [0]*len(s2)

    return lastCol[-1]
                


    # Return results
    return None


def main():
    print editdistance(["1","2","3"], ["1","2","3"])
    print

    print editdistance(["1","3"], ["1","2","3"])
    print

    print editdistance(["1","2","3"], ["1","3"])
    print

    print editdistance(["1","2","3"], ["1","3","3"])
    print

    print editdistance(["1","2","3"], ["1","3","4"])
    print


if __name__ == "__main__":
    main()
