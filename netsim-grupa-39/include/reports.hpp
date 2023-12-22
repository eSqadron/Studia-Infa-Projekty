//
// Created by Goon on 05.01.2021.
//

#ifndef NET_SIMULATION_REPORTS_HPP
#define NET_SIMULATION_REPORTS_HPP

#include "factory.hpp"
#include <iostream>


void generate_structure_report(const Factory& f, std::ostream& os);

void generate_simulation_turn_report(const Factory& f, std::ostream& os, Time t);

#endif //NET_SIMULATION_REPORTS_HPP
