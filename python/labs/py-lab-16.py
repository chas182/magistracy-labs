
def custom_xrange(*args):
    if len(args) == 1:
        start, stop, step = 0, args[0], 1
    elif len(args) == 2:
        start, stop, step = args[0], args[1], 1
    elif len(args) == 3:
        start, stop, step = args
    else:
        raise TypeError('custom_xrange() requires 1-3 int arguments')

    if step == 0:
        raise ValueError('custom_xrange() step must not be zero')

    cur = start
    while (step > 0 and cur < stop) or (step < 0 and cur > stop):
        yield cur
        cur += step


assert list(custom_xrange(3)) == list(xrange(3))
assert list(custom_xrange(1, 10)) == list(xrange(1, 10))
assert list(custom_xrange(1, 9, 2)) == list(xrange(1, 9, 2))
assert list(custom_xrange(9, 1, -2)) == list(xrange(9, 1, -2))
