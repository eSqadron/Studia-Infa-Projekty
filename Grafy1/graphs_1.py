from typing import List, Dict


def dfs_recursive(G: Dict[int, List[int]], s: int) -> List[int]:
    vis = []

    def DFSrecursive(G2: Dict[int, List[int]], s2: int, visited: List):
        visited.append(s2)
        try:
            for i in G2[s2]:
                if i not in visited:
                    visited = DFSrecursive(G2, i, visited)
        except KeyError:
            pass
        return visited

    vis = DFSrecursive(G, s, vis)

    return vis


def dfs_iterative(G: Dict[int, List[int]], s: int) -> List[int]:
    visited = []
    S = [s]
    while S:
        v = S.pop(-1)
        if v not in visited:
            visited.append(v)
            try:
                for u in reversed(G[v]):
                    S.append(u)
            except KeyError:
                pass

    return visited


def adjmat_to_adjlist(adjmat: List[List[int]]) -> Dict[int, List[int]]:
    w = dict()
    for n in range(1, len(adjmat) + 1):
        i = adjmat[n - 1]
        t = []
        for j in range(1, len(i) + 1):
            if i[j - 1] != 0:
                for k in range(i[j - 1]):
                    t.append(j)

        if t != []:
            w[n] = t
    return w


def is_acyclic(G: Dict[int, List[int]]) -> bool:
    def DFSrecursive(G2: Dict[int, List[int]], s: int, visited: List):
        try:
            for j in G2[s]:
                if j not in visited:
                    visited.append(j)
                    visited = DFSrecursive(G2, j, visited)
        except KeyError:
            visited.append(s)

        return visited

    for i in G.keys():
        vis = []
        vis = DFSrecursive(G, i, vis)
        if i in vis:
            return False

    return True
