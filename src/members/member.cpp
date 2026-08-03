#include "member.h"
#include "../network/requests.h"
#include "../parser/parser.h"
#include <atomic>
#include <chrono>
#include <string>
#include <iostream>
#include <thread>
#include <vector>

namespace {

const size_t FETCH_WORKERS = 8;

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

void Member::fetchProfile(Requests &request) {
  if (username.empty()) {
    std::cout << "[WARN] Skipping profile fetch, empty username." << std::endl;
    return;
  }
  std::string url = "https://vjudge.net/user/" + username;
  std::cout << "[INFO] Fetching profile for " << username << "..." << std::endl;
  request.request(Method::Get, url, std::nullopt);
  if (request.response.status_code != 200) {
    std::cout << "[ERROR] Profile fetch for " << username << " returned status "
              << request.response.status_code << "." << std::endl;
    return;
  }

  try {
    Html page;
    if (!page.parse(request.response.text)) {
      std::cout << "[ERROR] Failed to parse profile HTML for " << username << "."
                << std::endl;
      return;
    }
    auto holder = page.select("script#profile-header-data");
    if (holder.empty()) {
      std::cout << "[ERROR] No profile-header-data on page for " << username
                << "." << std::endl;
      return;
    }
    json data = json::parse(holder[0].text());
    json counts = data.value("counts", json::object());
    solved = counts.value("acAll", 0);
    attempted = counts.value("attAll", 0);
    rank = data.value("ranks", json::object()).value("all", 0LL);
    std::cout << "[INFO] " << username << " rank=" << rank
              << " solved=" << solved << " attempted=" << attempted << "."
              << std::endl;
  } catch (const std::exception &ex) {
    std::cout << "[ERROR] Profile parse for " << username << " threw: "
              << ex.what() << "." << std::endl;
    return;
  }

  std::string solvedUrl = "https://vjudge.net/user/solveDetail/" + username;
  request.request(Method::Get, solvedUrl, std::nullopt);
  if (request.response.status_code != 200) {
    std::cout << "[ERROR] Solved list for " << username << " returned status "
              << request.response.status_code << "." << std::endl;
    return;
  }

  try {
    json detail = json::parse(request.response.text);
    json records = detail.value("acRecords", json::object());
    for (auto judge = records.begin(); judge != records.end(); ++judge) {
      for (const auto &problem : judge.value()) {
        solved_problems.push_back(judge.key() + "-" +
                                  problem.get<std::string>());
      }
    }
    std::cout << "[INFO] " << username << " listed "
              << solved_problems.size() << " solved problems." << std::endl;
  } catch (const std::exception &ex) {
    std::cout << "[ERROR] Solved list parse for " << username << " threw: "
              << ex.what() << "." << std::endl;
  }
}

std::vector<Member> makeMembers() {
  std::cout << "[INFO] Fetching members from the Programming Club..." << std::endl;
  std::vector<Member> result;
  std::string url = "https://vjudge.net/group/uffs_progclub";
  Requests request = Requests();
  request.request(Method::Get, url, std::nullopt);
  if (request.response.status_code != 200) {
    std::cout << "[ERROR] Group fetch returned status "
              << request.response.status_code << "." << std::endl;
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
    std::cout << "[INFO] Found " << data["memberBriefs"].size()
              << " members, fetching profiles..." << std::endl;
    for (const auto &brief : data["memberBriefs"]) {
      result.emplace_back(brief);
    }

    std::atomic<size_t> next{0};
    std::vector<std::thread> workers;
    for (size_t w = 0; w < FETCH_WORKERS; ++w) {
      workers.emplace_back([&result, &next] {
        Requests request;
        for (size_t i = next++; i < result.size(); i = next++) {
          result[i].fetchProfile(request);
        }
      });
    }
    for (std::thread &worker : workers) {
      worker.join();
    }
    std::cout << "[INFO] Loaded " << result.size() << " member profiles."
              << std::endl;
  } catch (const std::exception &ex) {
    std::cout << "[ERROR] makeMembers threw: " << ex.what() << "." << std::endl;
    return result;
  }

  return result;
}
