#pragme once

#include <iostream>
#include <string>
#include <map>
#include <set>
#include "Server.hpp"

struct ChannelMember
{
    Client *client;
    bool isOperator;
};

class Channel
{
    private:
        std::string name;
        std::string topic;
        std::string key; // mode k
        bool inviteOnly; // mode i
        bool topicRestricted; // mode t
        size_t userLimit; // mode l

        std::map<int, ChannelMember> members;
        std::set<int> invited;

    public:
        Channel(std::string name);
        ~Channel();
}