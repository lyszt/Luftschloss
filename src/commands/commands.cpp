#include "commands.h"
#include "../config/config.h"
#include "../members/member.h"
#include <algorithm>
#include <dpp/colors.h>
#include <dpp/discordclient.h>
#include <dpp/dispatcher.h>
#include <dpp/dpp.h>
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
          .set_color(dpp::colors::crimson_red)
          .set_image("https://media4.giphy.com/media/"
                     "v1."
                     "Y2lkPTc5MGI3NjExa2g0dWIxeGdjdjY5ZW5sNnlucGVlODB4aXAxMWx2e"
                     "jk4cjk5YmVjcyZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/"
                     "UsAXnKt1KWr9oGlzGH/giphy.gif")  

  auto members = botConfig->members;
  std::sort(
      members.begin(), members.end(),
      [](const Member &a, const Member &b) { return a.solved > b.solved; });

  std::vector<dpp::embed_field> fields;
  int fieldCount = 0;
  for (const auto &m : members) {
    if (m.username == "kalliddel") {
      leaderboardEmbed.set_description(std::format(
          "**Kaldwin** - Position: {} - {} solved.",
          fieldCount + 1, m.solved));
    }
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
      "Nulla enim alia re videmus populum Romanum orbem "
                           "subegisse terrarum nisi armorum exercitio, "
                           "disciplina castrorum usuque militiae.",
      "https://cdn.vjudge.net.cn/group/logo/26731?v=1743077818");

  e.reply(dpp::message().add_embed(leaderboardEmbed));
}

std::vector<Command> commands = {
    {"ping", "Sends a ping, to see if Alisa responds.", handlePing},
    {"leaderboard",
     "Gets the leaderboard from the Clube de Programação from UFFS.",
     handleVJudgeLeaderboard}};
