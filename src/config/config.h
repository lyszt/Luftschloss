#ifndef CONFIG_H
#define CONFIG_H
#include <cstdlib>
#include <string>
#include <dpp/dpp.h>
#include "../onready/onready.h"
#include "../commands/commandhandler.h"

struct BotConfig {
  std::string getToken();
  dpp::cluster* bot;
  BotConfig() {
    const char* token = std::getenv("DISCORD_TOKEN");
    bot = new dpp::cluster(token ? token : "");
    BotCommands command_handler(bot);
    OnReady on_ready(bot);
  }
};

#endif