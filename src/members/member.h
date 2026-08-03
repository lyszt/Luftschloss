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
        bool top_3 = false;
        long long rank = 0;
        int solved = 0;
        int attempted = 0;

        explicit Member(const json &brief);

        void fetchProfile();
};

std::vector<Member> makeMembers();

#endif
