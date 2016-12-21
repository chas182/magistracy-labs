from itertools import groupby


def pairs(arr):
    for i in range(0, len(arr), 2):
        yield arr[i:i + 2]


def compress(data):
    for k, g in groupby(data):
        yield len(list(g))
        yield k


def uncompress(data):
    if len(data) % 2 != 0:
        raise ValueError("List has incompatible size.")
    for k, v in pairs(data):
        if isinstance(k, int):
            for _ in range(k):
                yield v
        else:
            raise ValueError("List has wrong structure.")


l = ["a", "b", "b", "c", "b", "c", "c"]
print(["a", "b", "b", "c", "b", "c", "c"])
compressedL = list(compress(l))
print(compressedL)
print(list(uncompress(compressedL)))
