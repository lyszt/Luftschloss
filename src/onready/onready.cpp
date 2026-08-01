#include "onready.h"
#include "../commands/commands.h"
#include <dpp/dispatcher.h>
#include <dpp/once.h>
#include <dpp/presence.h>

OnReady::OnReady(dpp::cluster* bot) {
    bot->on_ready([bot](const dpp::ready_t&) {
        if (dpp::run_once<struct register_bot_commands>()) {
            for (const auto& command : commands) {
                bot->global_command_create(
                    dpp::slashcommand(command.name, command.description, bot->me.id)
                );
            }
        }

        dpp::activity activity(dpp::activity_type::at_competing, "You will all fall beneath the power of Lygon", "", "");
        bot->set_presence(dpp::presence(dpp::presence_status::ps_online, activity));
    });
}
