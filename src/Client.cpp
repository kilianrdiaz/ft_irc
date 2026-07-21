/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 13:51:03 by alejhern          #+#    #+#             */
/*   Updated: 2026/07/21 13:51:05 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <sys/socket.h>

Client::Client() : _fd(-1), _passOk(false), _registered(false) {}
Client::Client(int socket_fd) : _fd(socket_fd), _passOk(false), _registered(false) {}
Client::Client(const Client &other) : _fd(other._fd), _hostname(other._hostname), _recvBuffer(other._recvBuffer), _nickname(other._nickname), _username(other._username), _passOk(other._passOk), _registered(other._registered) {}
Client::~Client() {}

Client &Client::operator=(const Client &other)
{
    if (this != &other)
    {
        _fd = other._fd;
        _hostname = other._hostname;
        _recvBuffer = other._recvBuffer;
        _nickname = other._nickname;
        _username = other._username;
        _passOk = other._passOk;
        _registered = other._registered;
    }
    return *this;
}

// Getters && Setters

int Client::getFd() const { return _fd; }
std::string &Client::getBuffer() { return _recvBuffer; }
std::string Client::getNickname() const { return _nickname; }
std::string Client::getUsername() const { return _username; }
std::string Client::getRealname() const { return _realname; }
bool Client::getPassOk() { return _passOk; }
bool Client::getRegistered() { return _registered; }

void Client::setFd(int newFd) { _fd = newFd; }
void Client::setHostName(std::string hostname) { _hostname = hostname; }
void Client::setNickname(std::string newNick) { _nickname = newNick; }
void Client::setUsername(std::string newUser) { _username = newUser; }
void Client::setPassOk(bool value) { _passOk = value; }
void Client::setRegistered(bool value) { _registered = value; }
void Client::setRealname(std::string newRealname) { _realname = newRealname; }


std::string     Client::get_prefix() const 
{
    std::string username = _username.empty() ? "" : "!" + _username;
    std::string hostname = _hostname.empty() ? "" : "@" + _hostname;

    return _nickname + username + hostname;
}

void Client::write(const std::string &message)
{
    std::string fullMessage = message + "\r\n";
    ssize_t sent = send(_fd, fullMessage.c_str(), fullMessage.size(), 0);
    if (sent == -1)
        std::cerr << "Client <" << _fd << "> send() failed" << std::endl;
    else if (static_cast<size_t>(sent) < fullMessage.size())
        std::cerr << "Client <" << _fd << "> partial send (" << sent << "/" << fullMessage.size() << " bytes)" << std::endl;
}

void Client::reply(const std::string &reply)
{
    this->write(":" + get_prefix() + " " + reply);
}