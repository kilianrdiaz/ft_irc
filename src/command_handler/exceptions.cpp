/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exceptions.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 04:17:13 by alejhern          #+#    #+#             */
/*   Updated: 2026/07/22 13:00:00 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"
#include "command_excepts.hpp"

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

AlreadyRegisteredException::AlreadyRegisteredException(
    const std::string &nickname)
    : CommandException(
        ERR_ALREADYREGISTERED(nickname))
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
** NICK Exceptions
** ============================================================================
*/


AlreadyExistNicknameException::AlreadyExistNicknameException(
    const std::string &nickname,
    const std::string &used_nickname)
    : CommandException(
        ERR_NICKNAMEINUSE(nickname, used_nickname))
{
}

InvalidNicknameException::InvalidNicknameException(
    const std::string &nickname)
    : CommandException(
        ERR_NONICKNAMEGIVEN(nickname))
{
}

NoSuchNickException::NoSuchNickException(
    const std::string &nickname,
    const std::string &target)
    : CommandException(
        ERR_NOSUCHNICK(nickname, target))
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
        ERR_USERONCHANNEL(nickname, nickname, channel))
{
}

NotInChannelException::NotInChannelException(
    const std::string &nickname,
    const std::string &channel)
    : ChannelException(
        ERR_NOTONCHANNEL(nickname, channel))
{
}

TargetNotInChannelException::TargetNotInChannelException(
    const std::string &nickname,
    const std::string &target,
    const std::string &channel)
    : ChannelException(
        ERR_TARGETNOTONCHANNEL(nickname, target, channel))
{
}

InviteOnlyChannelException::InviteOnlyChannelException(
    const std::string &nickname,
    const std::string &channel)
    : ChannelException(
        ERR_INVITEONLYCHAN(nickname, channel))
{
}

BadChannelKeyException::BadChannelKeyException(
    const std::string &nickname,
    const std::string &channel)
    : ChannelException(
        ERR_BADCHANNELKEY(nickname, channel))
{
}

ChannelFullException::ChannelFullException(
    const std::string &nickname,
    const std::string &channel)
    : ChannelException(
        ERR_CHANNELISFULL(nickname, channel))
{
}

CannotSendToChannelException::CannotSendToChannelException(
    const std::string &nickname,
    const std::string &channel)
    : ChannelException(
        ERR_CANNOTSENDTOCHAN(nickname, channel))
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
        ERR_UNKNOWNMODE(nickname, mode))
{
}

QuitException::~QuitException() throw()
{
}