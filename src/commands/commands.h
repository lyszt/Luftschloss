#include "src/commands/commands.h"
#include <dpp/cluster.h>
#include <dpp/dispatcher.h>


BotCommands::BotCommands(dpp::cluster* bot) {
    bot->on_slashcommand([](const dpp::slashcommand_t& event) {
        if(event.command.get_command_name() == "ping") {
            event.reply("pong");
        }
    });
}