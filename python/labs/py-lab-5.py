# coding=utf-8

numbers = {1: 'one', 2: 'two', 3: 'three',
           4: 'four', 5: 'five', 6: 'six', 7: 'seven',
           8: 'eight', 9: 'nine', 10: 'ten'}

bottles = {1: "bottle", 2: "bottles"}

song = """
%s green %s hanging on the wall,
%s green %s hanging on the wall,
And if one green bottle should accidentally fall,
There’ll be %s green bottles hanging on the wall."""


def print_song(count):
    step = 1
    for n in xrange(count, 0, -step):
        print song % (
            numbers[n].capitalize(), bottles.get(n - step + 1, bottles.get(2)),
            numbers[n].capitalize(), bottles.get(n - step + 1, bottles.get(2)),
            numbers.get(n - step, 'no'))


print_song(10)
