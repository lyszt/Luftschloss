#include <dpp/dispatcher.h>
#include <iostream>
#include <dpp/dpp.h>
#include "src/config/config.h"


int main() {
    botConfig = new BotConfig();
    dpp::cluster* bot = botConfig->bot;
    bot->on_log(dpp::utility::cout_logger());

    std::cout << "Initializing alisa-v-zadzerkalli_core\n";
    bot->start(dpp::st_wait);
    return 0;
}
