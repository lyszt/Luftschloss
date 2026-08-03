#include "onmessage.h"
#include "../speech/speech.h"

bool verify_references_alisa(const dpp::message &message, dpp::cluster *bot)
{
    std::string msg = message.content;

    const bool mentioned = std::any_of(
        message.mentions.begin(), message.mentions.end(),
        [bot](const auto &m) { return m.first.id == bot->me.id; });

    return msg.find("alisa") != std::string::npos ||
           msg.find("obliterator") != std::string::npos ||
           mentioned;
}

OnMessage::OnMessage(dpp::cluster* bot) {
    bot->on_message_create([bot](const dpp::message_create_t& event) {
        if (event.msg.author.id != 779546493425287180 || event.msg.author.is_bot())
            return;

        if (!verify_references_alisa(event.msg, bot))
            return;

        event.reply(fetch_quick_answer(event.msg));
    });
}