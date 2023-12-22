//
// Created by OEM on 22.12.2020.
//

#ifndef NET_SIMULATION_PACKAGE_HPP
#define NET_SIMULATION_PACKAGE_HPP

#include <set>
#include "types.hpp"

class Package{
public:
    Package();
    Package(ElementID id);
    Package(Package&& other) : ID_( std::move(other.ID_)) {
        other.is_moved_ = true;
    };
    Package(const Package& other) = delete; // i To
    //Package(Package& other) = delete;

    ElementID get_id() const {return ID_;};

    Package& operator= (Package&& other) = default;

    ~Package();

private:
    ElementID ID_; //chyba const powinno być
    bool is_moved_ = false;
    inline static std::set<ElementID> assigned_IDs {};
    inline static std::set<ElementID> freed_IDs {};
};

#endif //NET_SIMULATION_PACKAGE_HPP
