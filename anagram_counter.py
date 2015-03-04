from __future__ import print_function
from operator import itemgetter
import fileinput

def order(line):
    chars = list(line)
    chars.sort(key=itemgetter(0))
    ordered = "".join(chars)
    return ordered

# read from file and prepare
words = []
for line in fileinput.input():
    word = []
    word.append(line.lower().strip())
    word.append(order(word[0]))
    words.append(word)

# sort
words.sort(key=itemgetter(1))

# print words

counter = 0
anagram = False
for i in range(1, len(words)):
    # if there are duplicates in dictionary
    if (words[i-1][1] == words[i][1]) and (words[i-1][0] != words[i][0]):
        if anagram is False:
            anagram = True
            # print('----')
            print()
            counter += 1
            print(words[i-1][0], end=" ")
        print (words[i][0], end=" ")
    else:
        anagram = False

print()
print(counter)
