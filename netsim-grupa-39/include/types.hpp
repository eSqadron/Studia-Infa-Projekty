//
// Created by OEM on 22.12.2020.
//

#ifndef NET_SIMULATION_TYPES_HPP
#define NET_SIMULATION_TYPES_HPP

#include <functional>

using ElementID = unsigned int;

// FIXME - naprawić typy
using Time = unsigned int;
using TimeOffset = unsigned int;
using ProbabilityGenerator = std::function<double(void)>;
#endif //NET_SIMULATION_TYPES_HPP
