#include "embed.h"

const std::string TITLE_TEXT = "The Obliterator responds...";
const std::string THUMB_URL = "https://i.pinimg.com/1200x/fa/f6/b8/faf6b849435c4fa90476f14c2be91097.jpg";
const uint32_t DEFAULT_COLOR = dpp::colors::crimson;

dpp::embed makeDefaultEmbed(dpp::message message) {
    return dpp::embed()
    .set_thumbnail(THUMB_URL)
    .set_description(message.content)
    .set_title(TITLE_TEXT)
    .set_color(DEFAULT_COLOR);
}

dpp::embed makeDefaultEmbed(std::string message) {
    return dpp::embed()
    .set_thumbnail(THUMB_URL)
    .set_description(message)
    .set_title(TITLE_TEXT)
    .set_color(DEFAULT_COLOR);
}