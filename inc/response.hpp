#ifndef IRC_RESPONSE_HPP
#define IRC_RESPONSE_HPP

#include <iostream>
#include <string>
#include <ctime>

/*
** ============================================================================
** ERROR RESPONSES
** ============================================================================
*/

#define ERR_NOTREGISTERED(source) \
    std::string("451 ") + source + " :You have not registered"

#define ERR_ALREADYREGISTERED(source) \
    std::string("462 ") + source + " :You may not reregister"

#define ERR_PASSWDMISMATCH(source) \
    std::string("464 ") + source + " :Password incorrect"

#define ERR_NONICKNAMEGIVEN(source) \
    std::string("431 ") + source + " :No nickname given"

#define ERR_ERRONEUSNICKNAME(source, nickname) \
    std::string("432 ") + source + " " + nickname + " :Erroneous nickname"

#define ERR_NICKNAMEINUSE(source, nickname) \
    std::string("433 ") + source + " " + nickname + " :Nickname is already in use"

#define ERR_UNKNOWNCOMMAND(source, command) \
    std::string("421 ") + source + " " + command + " :Unknown command"

#define ERR_NEEDMOREPARAMS(source, command) \
    std::string("461 ") + source + " " + command + " :Not enough parameters"

#define ERR_NOSUCHCHANNEL(source, channel) \
    std::string("403 ") + source + " " + channel + " :No such channel"

#define ERR_CANNOTSENDTOCHAN(source, channel) \
    std::string("404 ") + source + " " + channel + " :Cannot send to channel"

#define ERR_TOOMANYCHANNELS(source, channel) \
    std::string("405 ") + source + " " + channel + " :You have joined too many channels"

#define ERR_NOTONCHANNEL(source, channel) \
    std::string("442 ") + source + " " + channel + " :You're not on that channel"

#define ERR_USERONCHANNEL(source, nickname, channel) \
    std::string("443 ") + source + " " + nickname + " " + channel + " :is already on channel"

#define ERR_CHANNELISFULL(source, channel) \
    std::string("471 ") + source + " " + channel + " :Cannot join channel (+l)"

#define ERR_UNKNOWNMODE(source, mode) \
    std::string("472 ") + source + " " + mode + " :is unknown mode char to me"

#define ERR_INVITEONLYCHAN(source, channel) \
    std::string("473 ") + source + " " + channel + " :Cannot join channel (+i)"

#define ERR_BANNEDFROMCHAN(source, channel) \
    std::string("474 ") + source + " " + channel + " :Cannot join channel (+b)"

#define ERR_BADCHANNELKEY(source, channel) \
    std::string("475 ") + source + " " + channel + " :Cannot join channel (+k)"

#define ERR_CHANOPRIVSNEEDED(source, channel) \
    std::string("482 ") + source + " " + channel + " :You're not channel operator"

#define ERR_USERNOTINCHANNEL(source, nickname, channel) \
    std::string("441 ") + source + " " + nickname + " " + channel + " :They aren't on that channel"

#define ERR_NOSUCHNICK(source, nickname) \
    std::string("401 ") + source + " " + nickname + " :No such nick/channel"

#define ERR_USERSDONTMATCH(source) \
    std::string("502 ") + source + " :Cannot change mode for other users"


/*
** ============================================================================
** NUMERIC RESPONSES
** ============================================================================
*/

#define RPL_WELCOME(nickname) \
    std::string("001 ") + nickname + " :Welcome to the ft_irc network"

#define RPL_YOURHOST(nickname) \
    std::string("002 ") + nickname + " :Your host is ft_irc"

#define RPL_CREATED(nickname) \
    std::string("003 ") + nickname + " :This server was created today"

#define RPL_MYINFO(nickname) \
    std::string("004 ") + nickname + " ft_irc 1.0 o itkol"

#define RPL_NOTOPIC(nickname, channel) \
    std::string("331 ") + nickname + " " + channel + " :No topic is set"

#define RPL_TOPIC(nickname, channel, topic) \
    std::string("332 ") + nickname + " " + channel + " :" + topic

#define RPL_NAMREPLY(nickname, channel, users) \
    std::string("353 ") + nickname + " = " + channel + " :" + users

#define RPL_ENDOFNAMES(nickname, channel) \
    std::string("366 ") + nickname + " " + channel + " :End of /NAMES list"

#define RPL_INVITING(source, target, channel) \
    std::string("341 ") + source + " " + target + " " + channel


/*
** ============================================================================
** CLIENT MESSAGES
** ============================================================================
*/

#define MSG_JOIN(prefix, channel) \
    std::string(":") + prefix + " JOIN :" + channel

#define MSG_PART(prefix, channel, reason) \
    std::string(":") + prefix + " PART " + channel + " :" + reason

#define MSG_QUIT(prefix, message) \
    std::string(":") + prefix + " QUIT :" + message

#define MSG_PRIVMSG(prefix, target, message) \
    std::string(":") + prefix + " PRIVMSG " + target + " :" + message

#define MSG_NOTICE(prefix, target, message) \
    std::string(":") + prefix + " NOTICE " + target + " :" + message

#define MSG_KICK(prefix, channel, target, reason) \
    std::string(":") + prefix + " KICK " + channel + " " + target + " :" + reason

#define MSG_MODE(prefix, channel, modes, args) \
    std::string(":") + prefix + " MODE " + channel + " " + modes + " " + args

#define MSG_TOPIC(prefix, channel, topic) \
    std::string(":") + prefix + " TOPIC " + channel + " :" + topic


/*
** ============================================================================
** LOG
** ============================================================================
*/

static inline void log(const std::string &message)
{
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(
        buffer,
        sizeof(buffer),
        "%d-%m-%Y %H:%M:%S",
        timeinfo
    );

    std::cout
        << "\033[0;34m["
        << buffer
        << "]\033[0m "
        << message
        << std::endl;
}

#endif