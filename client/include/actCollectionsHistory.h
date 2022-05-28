#ifndef YAFR_ACTCOLLECTIONSHISTORY_H
#define YAFR_ACTCOLLECTIONSHISTORY_H

#include <string>

struct ActCollectionsHistory{
    std::string type; // delete, add
    int bookId;
};

#endif //YAFR_ACTCOLLECTIONSHISTORY_H