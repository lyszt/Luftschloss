#ifndef REQUESTS_H
#define REQUESTS_H

#include <cpr/response.h>
#include <cpr/session.h>
#include <cstdint>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum class Method { Get, Post, Put, Delete, Patch };

class Requests {
    public:
        cpr::Response response;
        std::int32_t timeout_ms = 20000;
        void request(Method method, std::string &url, std::optional<json> body);

    private:
        cpr::Session session;
};


#endif