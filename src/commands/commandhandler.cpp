#include "commandhandler.h"
#include "commands.h"
#include <dpp/dispatcher.h>

BotCommands::BotCommands(dpp::cluster* bot) {
    for (auto command : commands) {
        bot->on_slashcommand([command, bot](const dpp::slashcommand_t& event) {
            if (event.command.get_command_name() == command.name) {
                command.handler(event, bot);
            }
        });
    }
}
