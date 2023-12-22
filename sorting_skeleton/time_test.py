import sort
import random
from timeit import timeit

tab_sorted = list(range(700))
tab_reversed = list(range(700, 0, -1))
tab_eq = [1]*700
tab_rand = random.sample(range(0, 1000), 700)
n=200

t1_bubble = timeit("sort.bubblesort(tab_sorted)", number=n, globals=globals())
t2_bubble = timeit("sort.bubblesort(tab_reversed)", number=n, globals=globals())
t3_bubble = timeit("sort.bubblesort(tab_eq)", number=n, globals=globals())
t4_bubble = timeit("sort.bubblesort(tab_rand)", number=n, globals=globals())

print("bubblesort(tab_sorted): {}".format(t1_bubble/n))
print("bubblesort(tab_reversed): {}".format(t2_bubble/n))
print("bubblesort(tab_eq): {}".format(t3_bubble/n))
print("bubblesort(tab_rand): {}".format(t4_bubble/n))


t1_quick = timeit("sort.quicksort(tab_sorted)", number=n, globals=globals())
t2_quick = timeit("sort.quicksort(tab_reversed)", number=n, globals=globals())
t3_quick = timeit("sort.quicksort(tab_eq)", number=n, globals=globals())
t4_quick = timeit("sort.quicksort(tab_rand)", number=n, globals=globals())

print("quicksort(tab_sorted): {}".format(t1_quick/n))
print("quicksort(tab_reversed): {}".format(t2_quick/n))
print("quicksort(tab_eq): {}".format(t3_quick/n))
print("quicksort(tab_rand): {}".format(t4_quick/n))

