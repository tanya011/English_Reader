#include <iostream>
#include <fstream>
#include <bits/ios_base.h>

namespace userRepLocal {

    int getValue(){
        int buff;
        std::ifstream file("/home/tatyana/Programming/Проект Весна 2022/English_Reader/client/src/files/numColl");
        if(!file.good()){
            throw "error";
        }
        file >> buff;
        file.close();
        std::cout << "VALUE =" << buff << std::endl;
        return buff;
    }

    void newValue(int newValue) {
        std::ofstream file("/home/tatyana/Programming/Проект Весна 2022/English_Reader/client/src/files/numColl", std::ios_base::out | std::ios_base::trunc);
        file << newValue;
        file.close();
    }

}