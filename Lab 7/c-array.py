import re

rex: list[tuple[re.Pattern, str]] = [
    (re.compile("int ([a-zA-Z+])\\[([1-9+])\\]\\[([1-9+])\\]\\[([1-9+])\\];"), "declaration"),
    (re.compile("int ([a-zA-Z+]) = ([a-zA-Z])\\[([0-9+])\\]\\[([0-9+])\\]\\[([0-9+])\\];"), "access"),  # access
    (re.compile("([a-zA-Z+])\\[([0-9+])\\]\\[([0-9+])\\]\\[([0-9+])\\] = ([0-9+]);"), "assign"),
]


arrays: dict[str, tuple[int, int, int]] = {}
values: dict[tuple[str, int, int, int], int] = {}

while True:
    s = input()
    # pattern match string
    for r in rex:
        if r[0].match(s):
            if r[1] == "declaration":
                for iden, idx1, idx2, idx3 in r[0].findall(s):
                    arrays[iden] = (idx1, idx2, idx3)

            if r[1] == "access":
                for x, iden, idx1, idx2, idx3 in r[0].findall(s):
                    if idx1 < arrays[iden][0] and idx2 < arrays[iden][1] and idx3 < arrays[iden][2]:
                        print(values.get((iden, idx1, idx2, idx3), 0))
                    else:
                        print("invalid access")

            if r[1] == "assign":
                for iden, idx1, idx2, idx3, x in r[0].findall(s):
                    if idx1 < arrays[iden][0] and idx2 < arrays[iden][1] and idx3 < arrays[iden][2]:
                        values[(iden, idx1, idx2, idx3)] = x
                    else:
                        print("invalid access")

            continue
