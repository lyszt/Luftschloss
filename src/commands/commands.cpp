#include "commands.h"
#include "../network/requests.h"
#include <dpp/discordclient.h>
#include <dpp/dispatcher.h>
#include <dpp/dpp.h>
#include <optional>
#include <vector>

// ============= UTILS ===============
void handlePing(const dpp::slashcommand_t &e, const dpp::cluster *_bot) {
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
                             const dpp::cluster *bot) {
  std::string url = "https://vjudge.net/group/uffs_progclub";
  Requests leaderboardHtml = Requests();
  leaderboardHtml.request(Method::Get, url, std::nullopt);
  auto response = leaderboardHtml.response;

  std::string pageContent = "";
  if (response.status_code == 200) {
    pageContent = response.text;
  }

  if (pageContent != "") {
    e.reply(pageContent.substr(0, 1900));
  }
}

std::vector<Command> commands = {
    {"ping", "Sends a ping, to see if Alisa responds.", handlePing},
    {"leaderboard",
     "Gets the leaderboard from the Clube de Programação from UFFS.",
     handleVJudgeLeaderboard}};
