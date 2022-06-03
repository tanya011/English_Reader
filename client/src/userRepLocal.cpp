#include <iostream>
#include <fstream>
#include <bits/ios_base.h>

namespace userRepLocal {

    int getValue(){
        int buff;
        //TODO not absolute path
        std::ifstream file("yafr_files/files/numCollection.txt");
        if(!file.good()){
            throw "error";
        }
        file >> buff;
        file.close();
        std::cout << "VALUE =" << buff << std::endl;
        return buff;
    }

    void newValue(int newValue) {
        //TODO not absolute path
        std::ofstream file("/home/FrankReader/client/src/files/numColl", std::ios_base::out | std::ios_base::trunc);
        file << newValue;
        file.close();
    }

}