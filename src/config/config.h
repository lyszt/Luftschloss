#ifndef CONFIG_H
#define CONFIG_H
#include <cstdlib>
#include <string>
#include <dpp/dpp.h>
#include "../onready/onready.h"
#include "../onmessage/onmessage.h"
#include "../commands/commandhandler.h"
#include "../members/member.h"

struct BotConfig
{
  std::string getToken();
  dpp::cluster *bot;
  std::vector<Member> members;
  BotConfig()
  {
    const char *token = std::getenv("DISCORD_TOKEN");
    bot = new dpp::cluster(token ? token : "",
                           dpp::i_default_intents | dpp::i_message_content);
    members = makeMembers();
    std::sort(
        members.begin(), members.end(),
        [](const Member &a, const Member &b)
        { return a.solved > b.solved; });

    BotCommands command_handler(bot);
    OnReady on_ready(bot);
    OnMessage on_message(bot);
  }
};

extern BotConfig *botConfig;

#endif