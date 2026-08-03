#include "onmessage.h"
#include "../speech/speech.h"
#include "../embeds/embed.h"

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

        dpp::embed embed = makeDefaultEmbed(fetch_quick_answer(event.msg));
        event.reply(dpp::message().add_embed(embed));
    });
}