#ifndef REQUESTS_H
#define REQUESTS_H

#include <string>

enum class Method { Get, Post, Put, Delete, Patch };

class Requests {
    public:
        Method method;
        void request(std::string url);
};


#endif