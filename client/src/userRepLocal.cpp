#include <iostream>
#include <fstream>
#include <bits/ios_base.h>

namespace userRepLocal {
    int getValue(){
        int buff;
        std::ifstream file(NUM_COLL);
        if(!file.good()){
            throw std::runtime_error("Cant open sync file for Collection");
        }
        file >> buff;
        return buff;
    }
    void newValue(int newValue) {
        std::ofstream file(NUM_COLL, std::ios_base::out | std::ios_base::trunc);
        file << newValue;
    }
}