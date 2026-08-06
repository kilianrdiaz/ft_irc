#ifndef IRC_RESPONSE_HPP
#define IRC_RESPONSE_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <ctime>

#define COL_RESET  "\033[0m"
#define COL_INFO   "\033[0;32m"
#define COL_WARN   "\033[0;33m"
#define COL_ERROR  "\033[0;31m"
#define COL_EVENT  "\033[0;36m"

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

#define ERR_TARGETNOTONCHANNEL(source, target, channel) \
    std::string("441 ") + source + " " + target + " " + channel + " :They aren't on that channel"

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

#define RPL_WHOISUSER(askerNick, targetNick, username, host, realname) \
    std::string("311 ") + askerNick + " " + targetNick + " " + username + " " + host + " * :" + realname

#define RPL_WHOISSERVER(askerNick, targetNick, servername) \
    std::string("312 ") + askerNick + " " + targetNick + " " + servername + " :ft_irc network"

#define RPL_WHOISCHANNELS(askerNick, targetNick, channels) \
    std::string("319 ") + askerNick + " " + targetNick + " :" + channels

#define RPL_ENDOFWHOIS(askerNick, targetNick) \
    std::string("318 ") + askerNick + " " + targetNick + " :End of /WHOIS list"


/*
** ============================================================================
** CLIENT MESSAGES
** ============================================================================
*/

#define MSG_NICK(prefix, newNick) \
    std::string(":") + prefix + " NICK :" + newNick

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

#define MSG_LIST(prefix, channels) \
    std::string(":") + prefix + " LIST " + channels

#define MSG_INVITE(prefix, target, channel) \
    std::string(":") + prefix + " INVITE " + target + " :" + channel


/*
** ============================================================================
** LOG
** ============================================================================
*/

enum LogLevel
{
    INFO,
    WARN,
    ERROR,
    EVENT
};

template <typename T>
std::string toStr(const T &value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

static inline void log(LogLevel level, const std::string &message)
{
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);

    std::string color;
    std::string tag;

    switch (level)
    {
        case INFO:  color = COL_INFO;    tag = "INFO"; break;
        case WARN:  color = COL_WARN;    tag = "WARN"; break;
        case ERROR: color = COL_ERROR;   tag = "ERROR"; break;
        case EVENT: color = COL_EVENT;   tag = "EVENT"; break;
        default:    color = COL_RESET;   tag = "?????"; break;
    }

    std::cout << "\033[2;37m[" << buffer << "] "
               << color << "[" << tag << "] " << COL_RESET << message << std::endl;
}

#endif