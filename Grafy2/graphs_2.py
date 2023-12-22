from enum import Enum
from typing import Dict, List, Set, NamedTuple, Union, Optional
import networkx as nx

VertexID = int
EdgeID = int
AdjList = Dict[VertexID, List[VertexID]]
Distance = int


def neighbors(adjlist: AdjList, start_vertex_id: VertexID, max_distance: Distance) -> Set[VertexID]:
    class C(Enum):
        WHITE = 1
        GRAY = 2
        BLACK = 3

    w = []
    d = 1

    kolor = [C.WHITE] * max(adjlist)
    try:
        for u in adjlist[start_vertex_id]:
            kolor[u] = C.WHITE
    except KeyError:
        return set([])

    kolor[start_vertex_id] = C.GRAY
    Q = [start_vertex_id]
    levels = []
    l2 = 0
    l1 = 1
    l_count = 0
    while Q and d <= max_distance:
        u = Q.pop(0)
        try:
            l2 = l2 + len(adjlist[u])
            for v in adjlist[u]:
                if kolor[v] == C.WHITE:
                    kolor[v] = C.GRAY
                    Q.append(v)
                    w.append(v)
        except KeyError:
            pass
        kolor[u] = C.BLACK
        l_count += 1

        if l_count == l1:
            l1 = l2
            l_count = 0
            l2 = 0
            d += 1

    return set(w)


# Nazwana krotka reprezentująca segment ścieżki.
class TrailSegmentEntry(NamedTuple):
    v_start: VertexID
    v_end: VertexID
    edge: EdgeID
    weight: float


Trail = List[TrailSegmentEntry]


def load_multigraph_from_file(filepath: str) -> nx.MultiDiGraph:
    """Stwórz multigraf na podstawie danych o krawędziach wczytanych z pliku.

    :param filepath: względna ścieżka do pliku (wraz z rozszerzeniem)
    :return: multigraf
    """

    with open(filepath) as f:
        w = []
        for line in f:
            t_l = (line.strip("\n")).split()
            if t_l != [''] and t_l:
                w.append((int(t_l[0]), int(t_l[1]), float(t_l[2])))

        G = nx.MultiDiGraph()
        G.add_weighted_edges_from(w)
        return G


def find_min_trail(g: nx.MultiDiGraph, v_start: VertexID, v_end: VertexID) -> Optional[Trail]:
    """Znajdź najkrótszą ścieżkę w grafie pomiędzy zadanymi wierzchołkami.

    :param g: graf
    :param v_start: wierzchołek początkowy
    :param v_end: wierzchołek końcowy
    :return: najkrótsza ścieżka
    """

    for i in g:
        for j in g[i]:
            for k in g[i][j]:
                if g[i][j][k]['weight'] < 0:
                    return None

    if v_start not in g:
        return None
    if v_end not in g:
        return None

    def sort_Q(QQ):
        n = len(QQ)
        for i in range(n - 1):
            for j in range(n - 1):
                if QQ[j - 1][1] > QQ[j][1]:
                    t = QQ[j - 1]
                    QQ[j - 1] = QQ[j]
                    QQ[j] = t

    d = [float('inf')] * (max(g) + 1)
    w = [None] * max(g)

    d[v_start] = 0
    Q = [(v_i, d[v_i]) for v_i in g]
    while Q:
        u = Q.pop(0)
        for v in g[u[0]]:
            for i in g[u[0]][v]:
                if d[v] > d[u[0]] + g[u[0]][v][i]['weight']:
                    d[v] = d[u[0]] + g[u[0]][v][i]['weight']
                    w[v - 1] = TrailSegmentEntry(u[0], v, i, g[u[0]][v][i]['weight'])
                    for j in range(len(Q)):
                        if Q[j][0] == v:
                            Q[j] = v, d[v]
                    sort_Q(Q)

    if w == [None] * len(w):
        return None

    tt = v_end
    w2 = [w[v_end - 1]]

    while tt != v_start:
        tt = w2[-1][0]
        w2.append(w[tt - 1])

    return (w2[::-1])[1:]


def trail_to_str(trail: Trail) -> str:
    """Wyznacz reprezentację tekstową ścieżki.

    :param trail: ścieżka
    :return: reprezentacja tekstowa ścieżki
    """

    w = "{0} -[{1}: {2}]-> {3} ".format(trail[0][0], trail[0][2], trail[0][3], trail[0][1])
    s = trail[0][3]
    for i in trail[1:]:
        w = w + "-[{0}: {1}]-> {2} ".format(i[2], i[3], i[1])
        s += i[3]

    return w + " (total = {})".format(s)
