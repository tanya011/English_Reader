#include <iostream>
#include <fstream>
#include <bits/ios_base.h>

namespace userRepLocal {

    int getValue(){
        int buff;
        //TODO not absolute path
        std::ifstream file(NUM_COLL);
        if(!file.good()){
            throw "error";
        }
        file >> buff;
        std::cout << "VALUE =" << buff << std::endl;
        return buff;
    }

    void newValue(int newValue) {
        //TODO not absolute path
        std::ofstream file(NUM_COLL, std::ios_base::out | std::ios_base::trunc);
        file << newValue;
    }

}