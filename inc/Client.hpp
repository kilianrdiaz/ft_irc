#pragma once

#include <iostream>
#include <string>

class Client
{
  private:
	int _fd;
	std::string _hostname;
	std::string _servername;
	std::string _recvBuffer;
	std::string _nickname;
	std::string _username;
	std::string _realname;
	bool _passOk;
	bool _registered;

  public:
	Client();
	Client(int socket_fd);
	Client(const Client &other);
	~Client();
	Client &operator=(const Client &other);

	int getFd() const;
	std::string &getBuffer();
	std::string getHost() const;
	std::string getServername() const;
	std::string getNickname() const;
	std::string getUsername() const;
	std::string getRealname() const;
	std::string get_prefix() const;
	bool getPassOk() const;
	bool getRegistered() const;

	void setFd(int newFd);
	void setHostName(std::string hostname);
	void setServername(std::string servername);
	void setNickname(std::string newNick);
	void setUsername(std::string newUser);
	void setRealname(std::string newRealname);
	void setPassOk(bool value);
	void setRegistered(bool value);
	void write(const std::string &message);
};
