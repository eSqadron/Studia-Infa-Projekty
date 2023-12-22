import unittest

from networkx import dijkstra_path_length

import graphs_2


class TestGraphs(unittest.TestCase):
    def test_min_route(self):
        graph = graphs_2.load_multigraph_from_file("directed_graph.dat")
        self.assertEqual(dijkstra_path_length(graph, 1, 3), sum([graphs_2.find_min_trail(graph, 1, 3)[i][3] for i in range(len( graphs_2.find_min_trail(graph, 1, 3)))]))


if __name__ == '__main__':
    unittest.main()
