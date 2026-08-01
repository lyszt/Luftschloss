#include "commands.h"
#include <dpp/discordclient.h>
#include <dpp/dispatcher.h>
#include <dpp/dpp.h>
#include <vector>

void handlePing(const dpp::slashcommand_t &e, const dpp::cluster *_bot) {
  auto start = std::chrono::steady_clock::now();

    e.reply("Testing...", [start, e](const dpp::confirmation_callback_t&) {
    auto end = std::chrono::steady_clock::now();

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                  .count();

    const std::string message = std::string("Response in ") +
                               std::to_string(ms) +
                                "ms. I stand ready, sir.";
    e.edit_original_response(dpp::message(message));
    });
}

std::vector<Command> commands = {
    {"ping", "Sends a ping, to see if Alisa responds.", handlePing}};
