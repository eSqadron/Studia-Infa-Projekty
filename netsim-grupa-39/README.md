# [netsim](http://home.agh.edu.pl/~mdig/dokuwiki/doku.php?id=teaching:programming:soft-dev:topics:net-simulation:start)-grupa-39
## Etap 1.1 - [Półprodukty i ich przechowywanie](http://home.agh.edu.pl/~mdig/dokuwiki/doku.php?id=teaching:programming:soft-dev:topics:net-simulation:part_storage)
### klasa Package - DONE
* 3 konstruktory, nie przyjmujący nic, przyjmujący ID, przyjmujący inny obiekt typu Package
* prywatna zmienna ID_ ma być unsigned int. W przypadku konstruktora nic nie przyjmującego ma być przydzielane automatycznie, najmniejsze możliwe
  * alias ElementID na unsigned int, i to typu ElementID ma być zmienna ID_
  * uwzględnić fakt, że produkty w przyszłości będą zwlaniały ID
  * statyczne zbiory (set<ElementID>) assigned_IDs zawierający już przypisane ID i freed_IDs, ID które zostały zwolnione na końcu "kolejki" produkcyjnej
* operator przypisania =
* get_id (po prostu metoda zwracająca prywatną zmienną ID_)
 
 ### interfejs IPackageStockpile (w tym interfejsie wszystko ma być abstrakcyjne) - DONE
* alias const_iterator na typ std::list<Package>::const_iterator. Umożliwi to stosowanie IPackageStockpile::const_iterator w innych miejscach w kodzie. Ma to zapewnić uniwersalność. (Albo jakoś tak, nie do końca to rozumiem, wyjdzie w praniu o co tu chodzi)
* push(Package&&) - metoda przyjmująca produkt i dodająca go do kontenera
* begin(), end(), cbegin(), cend() - 4 metody pozwalające na uzyskanie dostępu “tylko do odczytu” do kontenera przechowującego produkty
* size() - zwracająza liczbę produktów
* empty() - metoda zwracająca wartość logiczną prawda, jeśli kontener nie zawiera żadnych elementów (w przeciwnym razie zwraca fałsz)
* ~IPackageStockpile() - wirtualny destruktor. (Po jaką cholerę to ja nie wiem ale kazali)

### interfejs IPackageQueue (rozszerza interfejs IPackageStockpile)
* pop() - metoda zdejmująca element z  kontenera. Usuwa element z kontenera i go zwraca (chyba ma być niezaimplementowane)
* zmienna queue_type_
  * typu PackageQueueType, który jest typem wyliczeniowym zawierającym "FIFO" i "LIFO"
  * metoda get_queue_type() zwracająca wartość queue_type_

### PackageQueue (realizacja interfejsu IPackageQueue)
* konstruktor przyjmujący typ kolejki jaki to ma być (FIFO lub LIFO typu PackageQueueType) i w zależności od typu tworzącyt odpowiednią strukturę
* implementacja wszystkich metod abstrakcyjnych z interfejsów
  * size
  * empty
  * push
  * pop

### Testy jednostkowe
* class PackageQueue - “wyciąganie” elementu: czy w dobrej kolejności, w zależności od typu kolejki?
* class Package -nadawanie ID: czy dwie nowoutworzone paczki mają różne ID?

## Etap 1.2 - [Węzły sieci](http://home.agh.edu.pl/~mdig/dokuwiki/doku.php?id=teaching:programming:soft-dev:topics:net-simulation:part_nodes)
###
## Etap 1.3 - [Fabryka](http://home.agh.edu.pl/~mdig/dokuwiki/doku.php?id=teaching:programming:soft-dev:topics:net-simulation:part_factory)
## Etap 2 - [Warstwa odczytu i zapisu danych](http://home.agh.edu.pl/~mdig/dokuwiki/doku.php?id=teaching:programming:soft-dev:topics:net-simulation:part_io)
## Etap 3 - [Warstwa prezentacji danych](http://home.agh.edu.pl/~mdig/dokuwiki/doku.php?id=teaching:programming:soft-dev:topics:net-simulation:part_reports)
## Etap 4 - [Symulacja](http://home.agh.edu.pl/~mdig/dokuwiki/doku.php?id=teaching:programming:soft-dev:topics:net-simulation:part_simulation)
