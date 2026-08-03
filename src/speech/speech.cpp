#include "speech.h"
#include "../config/config.h"
#include "../network/requests.h"
#include <format>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

const int PORT = 8931;

namespace {

std::string clamp_to_reply_limit(std::string text) {
  constexpr size_t limit = 1990;
  if (text.size() <= limit) {
    return text;
  }
  size_t cut = limit;
  while (cut > 0 && (static_cast<unsigned char>(text[cut]) & 0xC0) == 0x80) {
    --cut;
  }
  text.resize(cut);
  return text + "...";
}

std::string join_problems(const std::vector<std::string> &problems) {
  std::string joined;
  for (const std::string &problem : problems) {
    if (!joined.empty()) {
      joined += ", ";
    }
    joined += problem;
  }
  return joined;
}

std::string build_member_context() {
  std::string context;
  const std::vector<Member> &members = botConfig->members;
  for (size_t i = 0; i < members.size(); ++i) {
    const Member &member = members[i];
    context += std::format(
        "{}. {} ({}) solved {} problems, active {}. {}\n", i + 1, member.name,
        member.username, member.solved, member.active,
        join_problems(member.solved_problems));
  }
  return context;
}

}

std::string fetch_quick_answer(const dpp::message &message)
{
  std::string url = std::format("http://127.0.0.1:{}/speech/deepthink/", PORT);
  std::string prompt = std::format("{}\n-- CLUB MEMBERS\n{}-- MOST SOLVED\n{}\n",
                                   message.content, build_member_context(),
                                   join_problems(botConfig->most_solved));
  std::cout << "[INFO] Generating message for user, prompt is "
            << prompt.size() << " bytes." << std::endl;

  json payload = {{"prompt", prompt},
                  {"username", "discord:" + message.author.username},
                  {"light", true}};

  Requests request;
  request.request(Method::Post, url, payload);
  if(request.response.status_code == 403) {
    return "Sorry, I can't talk to you.";
  }

  try {
    json parsed = json::parse(request.response.text);
    std::string answer = parsed.value("response", "");
    if (answer.empty()) {
      answer = parsed.value("error", "");
    }
    if (!answer.empty()) {
      return clamp_to_reply_limit(answer);
    }
  } catch (const std::exception &ex) {
    std::cout << "[ERROR] Providence reply parse threw: " << ex.what() << "."
              << std::endl;
  }
  return "Providence did not answer.";
}
