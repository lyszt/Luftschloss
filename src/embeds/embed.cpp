#include "embed.h"

dpp::embed makeDefaultEmbed(dpp::message message) {
    return dpp::embed()
    .set_description(message.content)
    .set_title("Alisa V Zadzerkalli")
    .set_color(dpp::colors::crimson_red);
}

dpp::embed makeDefaultEmbed(std::string message) {
    return dpp::embed()
    .set_description(message)
    .set_title("Alisa V Zadzerkalli")
    .set_color(dpp::colors::crimson_red);
}