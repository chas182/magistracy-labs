
def simple_numbers(n):
    return [i for i in xrange(1, n, 2) if all([i % d for d in xrange(2, i)])]

print simple_numbers(30)
