import re


def revert(s):
    reversedWords = reversed(re.findall(r'[A-Za-z]+', s))
    sWithoutWords = re.sub(r'[A-Za-z]+', '%s', s)
    return sWithoutWords % tuple(reversedWords)

string = '_____The___Day_That_Never_Comes___'
print string
print revert('_____The___Day_That_Never_Comes___')
