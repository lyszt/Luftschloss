#include "member.h"
#include "../network/requests.h"
#include "../parser/parser.h"
#include <chrono>
#include <string>
#include <iostream>

namespace {

std::string activeSince(long long epochMs) {
  auto now = std::chrono::system_clock::now();
  long long nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(
                        now.time_since_epoch())
                        .count();
  long long secs = (nowMs - epochMs) / 1000;
  if (secs < 60) {
    return std::to_string(secs) + " sec ago";
  }
  if (secs < 3600) {
    return std::to_string(secs / 60) + " min ago";
  }
  if (secs < 86400) {
    return std::to_string(secs / 3600) + " hr ago";
  }
  long long days = secs / 86400;
  if (days < 30) {
    return std::to_string(days) + " days ago";
  }
  if (days < 365) {
    return std::to_string(days / 30) + " months ago";
  }
  return std::to_string(days / 365) + " years ago";
}

std::string roleLabel(int role) {
  switch (role) {
  case 2:
    return "Leader";
  case 1:
    return "Manager";
  default:
    return "Member";
  }
}

}

Member::Member(const json &brief) {
  username = brief.value("username", "");
  name = brief.value("nickName", "");
  role = roleLabel(brief.value("role", 0));
  active = activeSince(brief.value("lastSeenTime", 0LL));
}

std::vector<Member> members;

std::vector<Member> makeMembers() {
  std::cout << "[INFO] Fetching members from the Programming Club..." << std::endl;
  std::vector<Member> result;
  std::string url = "https://vjudge.net/group/uffs_progclub";
  Requests request = Requests();
  request.request(Method::Get, url, std::nullopt);
  if (request.response.status_code != 200) {
    return result;
  }

  try {
    Html page;
    if (!page.parse(request.response.text)) {
      return result;
    }
    auto holder = page.select("textarea[name=dataJson]");
    if (holder.empty()) {
      return result;
    }
    json data = json::parse(holder[0].text());
    for (const auto &brief : data["memberBriefs"]) {
      result.emplace_back(brief);
    }
  } catch ([[maybe_unused]] const std::exception &ex) {
    return result;
  }

  return result;
}
