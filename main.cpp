#include <dpp/dispatcher.h>
#include <iostream>
#include <dpp/dpp.h>
#include "src/config/config.h"


int main() {
    BotConfig config = BotConfig();
    config.bot->on_log(dpp::utility::cout_logger());

    std::cout << "Initializing alisa-v-zadzerkalli_core\n";
    return 0;
}
