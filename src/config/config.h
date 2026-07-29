#ifndef CONFIG_H
#define CONFIG_H
#include <cstdlib>
#include <string>
#include <dpp/dpp.h>

struct BotConfig {
  std::string getToken();
  dpp::cluster* bot;
  BotConfig() {
    const char* token = std::getenv("DISCORD_TOKEN");
    bot = new dpp::cluster(token ? token : "");
  }
};

#endif