#include "commands.h"
#include <dpp/dispatcher.h>
#include <string>
#include <vector>


void handlePing(const dpp::slashcommand_t& e) {
    e.reply("pong");
}


std::vector<Command> commands = {
    {"ping", "Sends a ping, to see if Alisa responds.", handlePing}   
};
