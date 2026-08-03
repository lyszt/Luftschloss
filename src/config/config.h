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
  std::vector<std::string> getTopKFrequent(const std::vector<std::string> &items,
                                           int k);
  dpp::cluster *bot;
  std::vector<Member> members;
  std::vector<std::string> most_solved;
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

    std::vector<std::string> allSolved;
    for (const Member &member : members)
    {
      allSolved.insert(allSolved.end(), member.solved_problems.begin(),
                       member.solved_problems.end());
    }
    most_solved = getTopKFrequent(allSolved, 10);

    BotCommands command_handler(bot);
    OnReady on_ready(bot);
    OnMessage on_message(bot);
  }
};

extern BotConfig *botConfig;

#endif