#include "./config.h"

BotConfig* botConfig = nullptr;

std::string BotConfig::getToken() {
    return this->bot->token;
}
