#!/usr/bin/python
# -*- coding: utf-8 -*-
# Jakub Mazur, Piotr Wojciechowski, Michał Spinczyk
from abc import ABC, abstractmethod
from typing import Optional, List, TypeVar
import re


class Product:
    def __init__(self, name: str, price: float) -> None:
        self.name = name
        self.price = price

    def __eq__(self, other) -> bool:
        if self.name != other.name:
            return False

        if self.price != other.price:
            return False

        return True

    def __hash__(self) -> int:
        return hash((self.name, self.price))

    @property
    def name(self) -> str:
        return self._name

    @name.setter
    def name(self, name) -> None:
        if re.fullmatch("[a-z, A-Z]+[0-9]+", name):
            self._name = name
        else:
            raise ValueError


class TooManyProductsFoundError(Exception):
    # Reprezentuje wyjątek związany ze znalezieniem zbyt dużej liczby produktów.
    pass


class Server(ABC):
    n_max_returned_entries = 3

    def __init__(self, products: List[Product]) -> None:
        super().__init__()

    def get_entries(self, n_letters=1) -> List[Product]:
        t = [x for x in self.get_all_entries() if re.fullmatch("[a-z, A-Z]{" + str(n_letters) + "}[0-9]{2,3}", x.name)]
        if len(t) > self.n_max_returned_entries:
            raise TooManyProductsFoundError
        else:
            return sorted(t, key=lambda x: x.price)

    @abstractmethod
    def get_all_entries(self) -> List[Product]:
        pass


ServerTypes = TypeVar('ServerTypes', bound=Server)


class ListServer(Server):
    def __init__(self, products: List[Product]) -> None:
        super().__init__(products)
        self.products = sorted(products, key=lambda x: x.price)

    def get_all_entries(self):
        return self.products


class MapServer(Server):
    def __init__(self, products: List[Product]) -> None:
        super().__init__(products)
        self.products = {k.name: k for k in products}

    def get_all_entries(self) -> List[Product]:
        return list(self.products.values())


class Client:
    def __init__(self, server: ServerTypes) -> None:
        self.server = server

    def get_total_price(self, n_letters: Optional[int]) -> Optional[float]:
        total_price = 0
        try:
            products_list = self.server.get_entries(n_letters)

        except TooManyProductsFoundError:
            return None

        if len(products_list) == 0:
            return None

        for i in range(len(products_list)):
            total_price += products_list[i].price
        return total_price
