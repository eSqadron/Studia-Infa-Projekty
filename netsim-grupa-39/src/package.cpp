//
// Created by OEM on 22.12.2020.
//
#include "package.hpp"
#include "types.hpp"

Package::Package() {
    if(freed_IDs.size() > 0){
        ID_ = *(freed_IDs.begin());
        freed_IDs.erase(freed_IDs.begin());
    }
    else{
        ElementID i = 1;
        while(assigned_IDs.find(i) != assigned_IDs.end()){
            ++i;
        }
        assigned_IDs.insert(i);
        ID_ = i;
    }
}

/*Package&& Package::operator=(Package&& other) {
    return std::move(other);
}*/

Package::Package(ElementID id) : ID_(id) {
    if(freed_IDs.find(id) != freed_IDs.end()){
        freed_IDs.erase(id);
        assigned_IDs.insert(id);
    }
    else if(assigned_IDs.find(id) != assigned_IDs.end()){
        throw "product with such id already exists!";
    }
    else{
        assigned_IDs.insert(id);
    }


}

Package::~Package()  {
    if(!is_moved_)
    {
        freed_IDs.insert(ID_);
        assigned_IDs.erase(ID_);
    }
    //else is_moved_ = false;
}
