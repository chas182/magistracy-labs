import re


def revert(s):
    reversed_words = reversed(re.findall(r'[A-Za-z]+', s))
    str_without_words = re.sub(r'[A-Za-z]+', '%s', s)
    return str_without_words % tuple(reversed_words)

string = '_____The___Day_That_Never_Comes___'
print string
print revert('_____The___Day_That_Never_Comes___')
