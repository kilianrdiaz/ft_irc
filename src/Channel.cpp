#include "Channel.hpp"
#include <sys/socket.h>

Channel::Channel(const std::string &name, const std::string &key)
    : name(name), key(key), inviteOnly(false), topicRestricted(false), userLimit(0)
{
}

Channel::Channel(const Channel &other)
    : name(other.name), topic(other.topic), key(other.key),
      inviteOnly(other.inviteOnly), topicRestricted(other.topicRestricted),
      userLimit(other.userLimit), members(other.members), invited(other.invited)
{
}

Channel &Channel::operator=(const Channel &other)
{
    if (this != &other)
    {
        name = other.name;
        topic = other.topic;
        key = other.key;
        inviteOnly = other.inviteOnly;
        topicRestricted = other.topicRestricted;
        userLimit = other.userLimit;
        members = other.members;
        invited = other.invited;
    }
    return *this;
}

Channel::~Channel() {}

void Channel::addMember(Client *client, bool asOperator)
{
    members[client->getFd()] = asOperator;
}

void Channel::removeMember(int fd)
{
    members.erase(fd);
}

bool Channel::isMember(int fd) const
{
    return members.find(fd) != members.end();
}

bool Channel::isOperator(int fd) const
{
    std::map<int, bool>::const_iterator it = members.find(fd);
    if (it == members.end())
        return false;
    return it->second;
}

void Channel::setOperator(int fd, bool value)
{
    std::map<int, bool>::iterator it = members.find(fd);
    if (it != members.end())
        it->second = value;
}

size_t Channel::memberCount() const
{
    return members.size();
}

std::map<int, bool> &Channel::getMembers()
{
    return members;
}

void Channel::invite(int fd)
{
    invited.insert(fd);
}

bool Channel::isInvited(int fd) const
{
    return invited.find(fd) != invited.end();
}

void Channel::removeInvite(int fd)
{
    invited.erase(fd);
}

std::string Channel::getName() const
{
    return name;
}

std::string Channel::getTopic() const
{
    return topic;
}

void Channel::setTopic(const std::string &newTopic)
{
    topic = newTopic;
}

bool Channel::getInviteOnly() const
{
    return inviteOnly;
}

void Channel::setInviteOnly(bool value)
{
    inviteOnly = value;
}

bool Channel::getTopicRestricted() const
{
    return topicRestricted;
}

void Channel::setTopicRestricted(bool value)
{
    topicRestricted = value;
}

std::string Channel::getKey() const
{
    return key;
}

void Channel::setKey(const std::string &newKey)
{
    key = newKey;
}

bool Channel::hasKey() const
{
    return !key.empty();
}

size_t Channel::getUserLimit() const
{
    return userLimit;
}

void Channel::setUserLimit(size_t limit)
{
    userLimit = limit;
}

bool Channel::hasUserLimit() const
{
    return userLimit > 0;
}

bool Channel::isFull() const
{
    return hasUserLimit() && memberCount() >= userLimit;
}

std::string Channel::getMemberList(const Server &server) const
{
    std::string memberList;

    for (std::map<int, bool>::const_iterator it = members.begin();
         it != members.end(); ++it)
    {
        Client *client = const_cast<Server &>(server).getClientByFd(it->first);

        if (!client)
            continue;

        if (!memberList.empty())
            memberList += " ";

        if (it->second)
            memberList += "@";

        memberList += client->getNickname();
    }

    return memberList;
}

void Channel::broadcast(const std::string &msg, int senderFd)
{
    for (std::map<int, bool>::iterator it = members.begin();
         it != members.end(); ++it)
    {
        if (it->first == senderFd)
            continue;

        send(it->first, msg.c_str(), msg.size(), 0);
    }
}
