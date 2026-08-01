#include "commands.h"
#include "../config/config.h"
#include "../members/member.h"
#include <dpp/colors.h>
#include <dpp/discordclient.h>
#include <dpp/dispatcher.h>
#include <dpp/dpp.h>
#include <algorithm>
#include <format>
#include <functional>
#include <vector>

// ============= UTILS ===============
void handlePing(const dpp::slashcommand_t &e,
                [[maybe_unused]] const dpp::cluster *_bot) {
  auto start = std::chrono::steady_clock::now();

  e.reply("Testing...", [start, e](const dpp::confirmation_callback_t &) {
    auto end = std::chrono::steady_clock::now();

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                  .count();

    const std::string message = std::string("Response in ") +
                                std::to_string(ms) + "ms. I stand ready, sir.";
    e.edit_original_response(dpp::message(message));
  });
}
// ============ LEADERBOARDS ===========

void handleVJudgeLeaderboard(const dpp::slashcommand_t &e,
                             [[maybe_unused]] const dpp::cluster *bot) {
  dpp::embed leaderboardEmbed =
      dpp::embed()
          .set_title("Clube de Programação - Leaderboard")
          .set_url("https://vjudge.net/group/uffs_progclub")
          .set_color(dpp::colors::purple_amethyst)
          .set_image("https://i.pinimg.com/originals/11/56/71/"
                     "115671a1a70292994ba9ed0e000dfc27.gif")
          .set_description("Nulla enim alia re videmus populum Romanum orbem "
                           "subegisse terrarum nisi armorum exercitio, "
                           "disciplina castrorum usuque militiae.");

  auto members = botConfig->members;
  std::sort(
      members.begin(), members.end(),
      [](const Member &a, const Member &b) { return a.solved > b.solved; });

  std::vector<dpp::embed_field> fields;
  int fieldCount = 0;
  for (const auto &m : members) {
    if (fieldCount >= 25) {
      break;
    }
    fields.push_back({
        std::format("{}. {}", fieldCount + 1, m.name),
        std::format("{} solved - Active {}.", m.solved, m.active),
        true,
    });
    fieldCount++;
  }
  leaderboardEmbed.fields = fields;

  leaderboardEmbed.set_footer(
      "[...] - See more at VJudge.",
      "https://cdn.vjudge.net.cn/group/logo/26731?v=1743077818");

  e.reply(dpp::message().add_embed(leaderboardEmbed));
}

std::vector<Command> commands = {
    {"ping", "Sends a ping, to see if Alisa responds.", handlePing},
    {"leaderboard",
     "Gets the leaderboard from the Clube de Programação from UFFS.",
     handleVJudgeLeaderboard}};
