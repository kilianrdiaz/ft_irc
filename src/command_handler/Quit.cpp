#include "Command_handler.hpp"
#include "command_excepts.hpp"
#include "response.hpp"

QuitCommandHandler::QuitCommandHandler(Server &server, Client &client) : AbstractCommandHandler(server, client)
{
}

QuitCommandHandler::~QuitCommandHandler()
{
}

void QuitCommandHandler::execute(const std::vector<std::string> &params)
{
    std::string reason = params.empty() ? "Client Quit" : params[0];

    _client.write(MSG_QUIT(_client.getNickname(), reason));

    throw QuitException();
}