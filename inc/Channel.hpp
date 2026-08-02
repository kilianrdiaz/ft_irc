#pragma once

#include "Client.hpp"
#include "Server.hpp"
#include <iostream>
#include <map>
#include <set>
#include <string>

class	Server;

class Channel
{
  private:
	std::string name;
	std::string topic;
	std::string key;
	bool inviteOnly;
	bool topicRestricted;
	size_t userLimit;

	std::map<int, bool> members;
	std::set<int> invited;

  public:
	Channel(const std::string &name, const std::string &key = "");
	Channel(const Channel &other);
	Channel &operator=(const Channel &other);
	~Channel();

	// Gestión de miembros
	void addMember(Client *client, bool asOperator);
	void removeMember(int fd);
	bool isMember(int fd) const;
	bool isOperator(int fd) const;
	void setOperator(int fd, bool value);
	size_t memberCount() const;
	std::map<int, bool> &getMembers();

	// Invitaciones
	void invite(int fd);
	bool isInvited(int fd) const;
	void removeInvite(int fd);

	// Getters/setters básicos
	std::string getName() const;
	std::string getTopic() const;
	void setTopic(const std::string &newTopic);

	bool getInviteOnly() const;
	void setInviteOnly(bool value);

	bool getTopicRestricted() const;
	void setTopicRestricted(bool value);

	std::string getKey() const;
	std::string getMemberList(const Server &server) const;
	void setKey(const std::string &newKey);
	bool hasKey() const;

	size_t getUserLimit() const;
	void setUserLimit(size_t limit);
	bool hasUserLimit() const;
	bool isFull() const;

	void broadcast(const std::string &message, int excludeFd = -1);
};