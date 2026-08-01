#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include <functional>
#include <vector>
#include <dpp/dispatcher.h>

class Command {
public:
    const std::string name;
    const std::string description;
    const std::function<void(const dpp::slashcommand_t&, const dpp::cluster*)> handler;

    Command(std::string name,
            std::string description,
            std::function<void(const dpp::slashcommand_t&, const dpp::cluster*)> handler)
        : name(std::move(name)),
          description(std::move(description)),
          handler(std::move(handler))
    {}
};

extern std::vector<Command> commands;

#endif
