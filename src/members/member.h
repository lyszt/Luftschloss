#ifndef MEMBER_H
#define MEMBER_H

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;

class Member {
    public:
        std::string role;
        std::string name;
        std::string username;
        std::string active;

        explicit Member(const json &brief);
};

std::vector<Member> makeMembers();

#endif
