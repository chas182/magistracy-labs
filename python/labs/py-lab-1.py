

def array_cumulative_sum(arr):
    res = 0
    for number in arr:
        yield res
        res += number
    yield res


b = [1, 2, 3, 10, 14]

print list(array_cumulative_sum(b))

b = [1, 1, 1, 1, 1]

print list(array_cumulative_sum(b))
