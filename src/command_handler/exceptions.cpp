/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exceptions.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 04:17:13 by alejhern          #+#    #+#             */
/*   Updated: 2026/07/21 04:17:15 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"
#include "command_excepts.hpp"
#include <map>

/*
** ============================================================================
** CommandException
** ============================================================================
*/

CommandException::CommandException(const std::string &message)
    : _message(message)
{
}

CommandException::~CommandException() throw()
{
}

const char *CommandException::what() const throw()
{
    return _message.c_str();
}


/*
** ============================================================================
** Generic Command Exceptions
** ============================================================================
*/

CommandNotFoundException::CommandNotFoundException(
    const std::string &nickname,
    const std::string &command)
    : CommandException(
        ERR_UNKNOWNCOMMAND(nickname, command))
{
}

InvalidParametersException::InvalidParametersException(
    const std::string &nickname,
    const std::string &command)
    : CommandException(
        ERR_NEEDMOREPARAMS(nickname, command))
{
}


NotRegisteredException::NotRegisteredException(
    const std::string &nickname)
    : CommandException(
        ERR_NOTREGISTERED(nickname))
{
}

/*
** ============================================================================
** PASS Exceptions
** ============================================================================
*/

AlreadyPassedException::AlreadyPassedException(
    const std::string &nickname)
    : CommandException(
        ERR_ALREADYREGISTERED(nickname))
{
}

InvalidPasswordException::InvalidPasswordException(
    const std::string &nickname)
    : CommandException(
        ERR_PASSWDMISMATCH(nickname))
{
}


/*
** ============================================================================
** USER Exceptions
** ============================================================================
*/

InvalidUsernameException::InvalidUsernameException(
    const std::string &nickname)
    : CommandException(
        ERR_NONICKNAMEGIVEN(nickname))
{
}

AlreadyRegisteredException::AlreadyRegisteredException(
    const std::string &nickname)
    : CommandException(
        ERR_ALREADYREGISTERED(nickname))
{
}


/*
** ============================================================================
** NICK Exceptions
** ============================================================================
*/

InvalidNicknameException::InvalidNicknameException(
    const std::string &nickname)
    : CommandException(
        ERR_NONICKNAMEGIVEN(nickname))
{
}

NicknameInUseException::NicknameInUseException(
    const std::string &nickname)
    : CommandException(
        ERR_NICKNAMEINUSE(nickname))
{
}


/*
** ============================================================================
** PING Exceptions
** ============================================================================
*/

InvalidPingException::InvalidPingException(
    const std::string &nickname)
    : CommandException(
        ERR_NEEDMOREPARAMS(nickname, "PING"))
{
}


/*
** ============================================================================
** Channel Exceptions
** ============================================================================
*/

ChannelException::ChannelException(
    const std::string &message)
    : CommandException(message)
{
}

InvalidChannelException::InvalidChannelException(
    const std::string &nickname,
    const std::string &channel)
    : ChannelException(
        ERR_NOSUCHCHANNEL(nickname, channel))
{
}

NotPrivilegedException::NotPrivilegedException(
    const std::string &nickname,
    const std::string &channel)
    : ChannelException(
        ERR_CHANOPRIVSNEEDED(nickname, channel))
{
}

AlreadyInChannelException::AlreadyInChannelException(
    const std::string &nickname,
    const std::string &channel)
    : ChannelException(
        ERR_CANNOTSENDTOCHAN(nickname, channel))
{
}

NotInChannelException::NotInChannelException(
    const std::string &nickname,
    const std::string &channel)
    : ChannelException(
        ERR_NOTONCHANNEL(nickname, channel))
{
}


/*
** ============================================================================
** MODE Exceptions
** ============================================================================
*/

InvalidModeException::InvalidModeException(
    const std::string &nickname,
    const std::string &mode)
    : CommandException(
        ERR_NEEDMOREPARAMS(nickname, mode))
{
}