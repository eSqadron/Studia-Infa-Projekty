# problem-komowojazera-tsp-grupa-39
problem-komowojazera-tsp-grupa-39 created by GitHub Classroom

[Konspekt Laboratorium](http://home.agh.edu.pl/~mdig/dokuwiki/lib/exe/fetch.php?media=teaching:programming:ads:topics:tsp_text.pdf)

[Wskazówski i przykłady](http://home.agh.edu.pl/~mdig/dokuwiki/doku.php?id=teaching:programming:ads:topics:tsp)

# Etapy Rozwiązania

### Cz I. Macierz Kosztów:

W pierwszej części uzupełnij klasę reprezentująca macierz kosztów CostMatrix. Klasa przechowuje macierz oraz implementuje następujące operacje na macierzy:
1. ~~get_min_values_in_rows() -znajdź minimalne wartości w wierszach~~
   * Zaimplementowane, trzeba sprawdzić działanie
   * test jednostkowy napisany, wszystko działa
1. ~~get_min_values_in_cols() - znajdx minimalne wartości w kolumnach~~
   * Zaimplementowane, działa
1. reduce_rows() - redukcja minimalnych warto±ci w wierszach
   * Zaimplementowane, trzeba sprawdzić działanie  
1. reduce_cols() - redukcja minimalnych warto±ci w kolumnach
   * Zaimplementowane, wydaje się że nie działa???
1. ~~get_vertex_cost() - znajdź koszt nieodwiedzenia danego wierzchołka, tj.: znajddź sumę minimalnych wartości dla danej pary wierzchołków, patrz KROK 2 - wybór przej±cia.~~
   * Zaimplementowane, sprawdzone, gotowe

### Cz II: 

W drugiej cz¦±ci uzupeªnij klas¦ reprezentuj¡c¡ jedn¡ gaª¡¹ rozwi¡zania. Klasa przechowuje macierz
kosztów i implementuje metody potrzebne do znalezienia rozwi¡zania w ka»dej iteracji:
1. reduce_cost_matrix() - redukcja macierzy (KROK 1): funkcja zwraca sumę zredukowanych wartości
   * Do sprawdzenia razem z testem
1. ~~choose_new_vertex() - wybór nowego przejścia (KROK 2)~~
1. update_cost_matrix() - aktualizacja macierzy kosztów (KROK 3): zabronienie powrotnego przejścia oraz sprawdzenie ewentualnych cykli
1. ~~get_path() - dla zredukowanej macierzy kosztów 2 × 2 oraz dotychczasowego rozwi¡zania, zwró¢ ±cie»k¦ ko«cow¡ (rozwi¡zanie) w postaci ci¡gu kolejnych etykiet wierzchołków~~

### Cz III. Pełne rozwiązanie
   * Zrobione do sprawdzenia
