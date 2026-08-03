#pragma once

#include "Channel.hpp"
#include "Client.hpp"
#include <fcntl.h>
#include <iostream>
#include <map>
#include <poll.h>
#include <string>
#include <unistd.h>
#include <vector>

class	Client;
class	Channel;

class Server
{
  private:
	int port;
	std::string password;
	int serSocketFd;
	static bool sig;

	std::map<int, Client *> clients;
	std::vector<struct pollfd> fds;
	std::map<std::string, Channel *> channels;

  public:
	Server(int port, std::string password);
	Server(const Server &other);
	Server &operator=(const Server &other);
	~Server();

	void serverInit();
	void serSocket();
	void acceptNewClient();
	void receiveNewData(int fd);
	void replyToClient(int fd, const std::string &message);
	void tryRegisterClient(Client &client);
	Client *searchNickname(const std::string &nickname, int excludeFd = -1);
	std::string getPassword() const;
	std::map<int, Client *> getClients();
	std::map<std::string, Channel *> getChannels();
	Channel *getChannelByName(const std::string &channelName);
	void addChannel(Channel *channel);
	void removeChannel(const std::string &channelName);
	std::string listChannels(int targetFd) const;

	Client *getClientByFd(int fd);

	static void signalHandler(int signum);

	void clearClient(int fd);

	void removeClientFromChannels(Client &client, const std::string &message);
};