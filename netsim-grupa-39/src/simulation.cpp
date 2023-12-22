//
// Created by OEM on 07.01.2021.
//

#include "simulation.hpp"

void simulate(Factory& f, TimeOffset d, std::function<void(Factory&, Time)> rf) {
    for (unsigned int t = 1; t <=d ; ++t) {

        //1 Pojawianie się na rampach
        f.do_deliveries(t);


        //2 przekazanie produktów
        f.do_package_passing();


        //3 przetworzenie półproduktów
        f.do_work(t);


        //4 raportowanie
        rf(f, t);
    }
}
