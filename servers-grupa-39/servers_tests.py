#Jakub Mazur, Piotr Wojciechowski, Michał Spinczyk
import unittest
from collections import Counter

from servers import ListServer, Product, Client, MapServer, TooManyProductsFoundError

server_types = (ListServer, MapServer)


class ServerTest(unittest.TestCase):
    def test_get_entries_returns_sorted_list(self):
        products = [Product('P12', 1), Product('PP234', 3), Product('PP235', 2), Product('PP26', 4),
                    Product('PP26890', 4)]
        for server_type in server_types:
            server = server_type(products)
            entries = server.get_entries(2)
            self.assertEqual(len(entries), 3)
            self.assertEqual(entries[0], products[2])
            self.assertEqual(entries[1], products[1])
            self.assertEqual(entries[2], products[3])

    def test_get_entries_exception(self):
        products = [Product('PP12', 1), Product('PP234', 3), Product('PP235', 2), Product('PP26', 4),
                    Product('PP26890', 4)]
        for server_type in server_types:
            server = server_type(products)
            self.assertRaises(TooManyProductsFoundError, lambda: server.get_entries(2))

    def test_get_total_price_if_exception(self):
        products = [Product('PP12', 1), Product('PP234', 3), Product('PP235', 2), Product('PP26', 4),
                    Product('PP26890', 4)]
        for server_type in server_types:
            server = server_type(products)
            client = Client(server)
            cost = client.get_total_price(2)
            self.assertEqual(None, cost)

    def test_get_total_price_if_no_match(self):
        products = [Product('PP12', 1), Product('PP234', 3), Product('PP235', 2), Product('PP26', 4),
                    Product('PP26890', 4)]
        for server_type in server_types:
            server = server_type(products)
            client = Client(server)
            cost = client.get_total_price(1)
            self.assertEqual(None, cost)


# TODO - usunąć przed wysłaniem
class TestsToDelete(unittest.TestCase):
    def test_total_price_for_normal_execution(self):
        products = [Product('PP234', 2), Product('PP235', 3)]
        for server_type in server_types:
            server = server_type(products)
            client = Client(server)
            self.assertEqual(5, client.get_total_price(2))

    def test_get_entries_returns_proper_entries(self):
        products = [Product('P12', 1), Product('PP234', 2), Product('PP235', 1)]
        for server_type in server_types:
            server = server_type(products)
            entries = server.get_entries(2)
            self.assertEqual(Counter([products[2], products[1]]), Counter(entries))

    def test_product_name(self):
        #products = [Product('12', 1), Product('PP234', 2), Product('PP235', 1)]
        self.assertRaises(ValueError, lambda: Product('12', 1))
        self.assertRaises(ValueError, lambda: Product('ppp', 1))


if __name__ == '__main__':
    unittest.main()
