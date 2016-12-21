

def restrict_range(arr, rang):
    return [max(rang[0], min(rang[1], item)) for item in arr]


print(restrict_range([1, 2, 3, 4, 5, 6, 7, 8, 9, 10], (3, 6)))
