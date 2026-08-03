#include "speech.h"
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

}

std::string fetch_quick_answer(const dpp::message &message)
{
  std::string url = std::format("http://127.0.0.1:{}/speech/deepthink/", PORT);
  std::cout << "[INFO] Generating message for user." << std::endl;

  json payload = {{"prompt", message.content},
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
