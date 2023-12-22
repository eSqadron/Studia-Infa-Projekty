#include "library.h"

#include <iostream>

void funkcja_void(void){
    std::cout << global;
}


void hello() {
    funkcja_void();
}
