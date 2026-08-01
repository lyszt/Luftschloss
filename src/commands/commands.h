#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include <functional>
#include <vector>
#include <dpp/dispatcher.h>

struct Command {
    std::string name;
    std:: string description;
    std::function<void(const dpp::slashcommand_t&)> handler;
};

extern std::vector<Command> commands;

#endif
