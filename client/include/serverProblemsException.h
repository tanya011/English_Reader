#ifndef YAFR_SERVERPROBLEMSEXCEPTION_H
#define YAFR_SERVERPROBLEMSEXCEPTION_H

#include <exception>

struct ServerProblemsException : public std::exception {
    const char * what () const throw () {
        return "C++ Exception";
    }
};

#endif //YAFR_SERVERPROBLEMSEXCEPTION_H
