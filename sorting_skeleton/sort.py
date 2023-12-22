# Jakub Mazur, 402377
from copy import deepcopy
from typing import Tuple


def quicksort(tab: list, lo=0, hi=None) -> list:
    def quicksort2(tab: list, lo=0, hi=None) -> None:
        if hi is None:
            hi = len(tab) - 1

        if lo < hi:
            pivot = tab[hi]
            i = lo

            for j in range(lo, hi):
                if tab[j] < pivot:
                    t = tab[j]
                    tab[j] = tab[i]
                    tab[i] = t
                    i += 1
            t = tab[i]
            tab[i] = tab[hi]
            tab[hi] = t

            p = i
            quicksort2(tab, lo, p - 1)
            quicksort2(tab, p + 1, hi)

    tt = deepcopy(tab)
    quicksort2(tt, lo, hi)
    return tt


# def quicksort(tab: list, lo=0, hi=None) -> None:
#     if hi is None:
#         hi = len(tab) - 1
#
#     if lo < hi:
#         pivot = tab[round((hi - lo) / 2)]
#         i = lo
#         j = hi
#         p = 0
#         while p != j:
#             while tab[i] < pivot or i == hi:
#                 i = i + 1
#             while tab[j] > pivot or j == lo:
#
#                 j = j - 1
#             if i >= j:
#                 p = j
#             t = tab[i]
#             tab[i] = tab[j]
#             tab[j] = t
#
#         quicksort(tab, lo, p)
#         quicksort(tab, p + 1, hi)

#[1,2,3,4,5,6]
def bubblesort(t: list) -> Tuple[list, float]:
    tab = deepcopy(t)
    p = 0
    n = len(tab)
    while n > 1:
        puste_p = 0
        for i in range(n - 1):
            p += 1
            if tab[i + 1] < tab[i]:
                t = tab[i + 1]
                tab[i + 1] = tab[i]
                tab[i] = t
            else:
                puste_p += 1
        if puste_p == n - 1:
            return tab, p
        n -= 1

    return tab, p
