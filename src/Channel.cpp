#include "Channel.hpp"

#include "Channel.hpp"

Channel::Channel(const std::string &name) : name(name), inviteOnly(false), topicRestricted(false), userLimit(0)
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
    ChannelMember member;
    member.client = client;
    member.isOperator = asOperator;
    members[client->getFd()] = member;
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
    std::map<int, ChannelMember>::const_iterator it = members.find(fd);
    if (it == members.end())
        return false;
    return it->second.isOperator;
}

void Channel::setOperator(int fd, bool value)
{
    std::map<int, ChannelMember>::iterator it = members.find(fd);
    if (it != members.end())
        it->second.isOperator = value;
}

size_t Channel::memberCount() const
{
    return members.size();
}

std::map<int, ChannelMember> &Channel::getMembers()
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