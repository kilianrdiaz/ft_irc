/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_excepts.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 04:22:26 by alejhern          #+#    #+#             */
/*   Updated: 2026/07/21 04:22:31 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_EXCEPTS_HPP
# define COMMAND_EXCEPTS_HPP

# include <exception>
# include <string>

/*
 * ========================================================================== *
 * Exceptions
 * ========================================================================== *
 */

class CommandException : public std::exception
{
  protected:
	std::string _message;

  public:
	CommandException(const std::string &message);
	virtual ~CommandException() throw();

	virtual const char *what() const throw();
};

/*
 * Generic command exceptions
 */

class CommandNotFoundException : public CommandException
{
  public:
	CommandNotFoundException(const std::string &nickname,
		const std::string &command);
};

class InvalidParametersException : public CommandException
{
  public:
	InvalidParametersException(const std::string &nickname,
		const std::string &command);
};

class AlreadyRegisteredException : public CommandException
{
  public:
	AlreadyRegisteredException(const std::string &nickname);
};

class NotRegisteredException : public CommandException
{
  public:
	NotRegisteredException(const std::string &nickname);
};

/*
 * PASS exceptions
 */

class AlreadyPassedException : public CommandException
{
  public:
	AlreadyPassedException(const std::string &nickname);
};

class InvalidPasswordException : public CommandException
{
  public:
	InvalidPasswordException(const std::string &nickname);
};

/*
 * NICK exceptions
 */

class InvalidNicknameException : public CommandException
{
  public:
	InvalidNicknameException(const std::string &nickname);
};

class AlreadyExistNicknameException : public CommandException
{
  public:
	AlreadyExistNicknameException(const std::string &nickname,
		const std::string &used_nickname);
};

class NoSuchNickException : public CommandException
{
  public:
	NoSuchNickException(const std::string &nickname, const std::string &target);
};

/*
 * PING exceptions
 */

class InvalidPingException : public CommandException
{
  public:
	InvalidPingException(const std::string &nickname);
};

/*
 * Channel exceptions
 */

class ChannelException : public CommandException
{
  public:
	ChannelException(const std::string &message);
};

class NoSuchChannelException : public ChannelException
{
  public:
	NoSuchChannelException(const std::string &nickname,
		const std::string &channel);
};

class InvalidChannelException : public ChannelException
{
  public:
	InvalidChannelException(const std::string &nickname,
		const std::string &channel);
};

class NotPrivilegedException : public ChannelException
{
  public:
	NotPrivilegedException(const std::string &nickname,
		const std::string &channel);
};

class AlreadyInChannelException : public ChannelException
{
  public:
	AlreadyInChannelException(const std::string &nickname,
		const std::string &channel);
};

class NotInChannelException : public ChannelException
{
  public:
	NotInChannelException(const std::string &nickname,
		const std::string &channel);
};

class InviteOnlyChannelException : public ChannelException
{
  public:
	InviteOnlyChannelException(const std::string &nickname,
		const std::string &channel);
};

class BadChannelKeyException : public ChannelException
{
  public:
	BadChannelKeyException(const std::string &nickname,
		const std::string &channel);
};

class ChannelFullException : public ChannelException
{
  public:
	ChannelFullException(const std::string &nickname,
		const std::string &channel);
};

/*
 * MODE exceptions
 */

class InvalidModeException : public CommandException
{
  public:
	InvalidModeException(const std::string &nickname, const std::string &mode);
};

/*
 * PRIVMSG exceptions
 */

class CannotSendToChannelException : public ChannelException
{
  public:
	CannotSendToChannelException(const std::string &nickname,
		const std::string &channel);
};

#endif