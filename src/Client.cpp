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

Client::Client() : fd(-1), passOk(false), registered(false) {}
Client::Client(int socket_fd) : fd(socket_fd), passOk(false), registered(false) {}
Client::Client(const Client &other) : fd(other.fd), address(other.address), recvBuffer(other.recvBuffer), nickname(other.nickname), username(other.username), passOk(other.passOk), registered(other.registered) {}
Client::~Client() {}

Client &Client::operator=(const Client &other)
{
    if (this != &other)
    {
        fd = other.fd;
        address = other.address;
        recvBuffer = other.recvBuffer;
        nickname = other.nickname;
        username = other.username;
        passOk = other.passOk;
        registered = other.registered;
    }
    return *this;
}

// Getters && Setters

int Client::getFd() { return fd; }
std::string &Client::getBuffer() { return recvBuffer; }
std::string Client::getNickname() { return nickname; }
std::string Client::getUsername() { return username; }
bool Client::getPassOk() { return passOk; }
bool Client::getRegistered() { return registered; }

void Client::setFd(int newFd) { fd = newFd; }
void Client::setIpAdd(std::string newAddress) { address = newAddress; }
void Client::setNickname(std::string newNick) { nickname = newNick; }
void Client::setUsername(std::string newUser) { username = newUser; }
void Client::setPassOk(bool value) { passOk = value; }
void Client::setRegistered(bool value) { registered = value; }


void Client::write(const std::string &message)
{
    std::string fullMessage = message + "\r\n";
    ssize_t sent = send(fd, fullMessage.c_str(), fullMessage.size(), 0);
    if (sent == -1)
        std::cerr << "Client <" << fd << "> send() failed" << std::endl;
    else if (static_cast<size_t>(sent) < fullMessage.size())
        std::cerr << "Client <" << fd << "> partial send (" << sent << "/" << fullMessage.size() << " bytes)" << std::endl;
}

void Client::reply(const std::string &code, const std::string &message)
{
    std::string nick = nickname.empty() ? "*" : nickname;
    write(":ircserv " + code + " " + nick + " " + message);
}