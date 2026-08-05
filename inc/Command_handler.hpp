/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Comand_handler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 22:45:43 by alejhern          #+#    #+#             */
/*   Updated: 2026/07/20 22:45:51 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HANDLER_HPP
# define COMMAND_HANDLER_HPP

# include "Client.hpp"
# include "Server.hpp"
# include "command_excepts.hpp"
# include <exception>
# include <string>
# include <vector>

typedef void	(*HandlerExecutor)(Server &, Client &,
		const std::vector<std::string> &);

struct			Command
{
	std::string name;
	std::vector<std::string> params;
};

class AbstractCommandHandler
{
  private:
	AbstractCommandHandler(const AbstractCommandHandler &);

	AbstractCommandHandler &operator=(const AbstractCommandHandler &);

  protected:
	Server &_server;
	Client &_client;

  public:
	AbstractCommandHandler(Server &server, Client &client);

	virtual ~AbstractCommandHandler();

	void setCommandName(const std::string commandName);

	const std::string &getCommandName() const;

	static Command parseLine(const std::string &line);

	static void executeCommand(Server &server, Client &client,
		const std::string commandName, const std::vector<std::string> &params);

	virtual void execute(const std::vector<std::string> &params) = 0;
};

/*
 * ========================================================================== *
 * PASS
 * ========================================================================== *
 */

class PassCommandHandler : public AbstractCommandHandler
{
  public:
	PassCommandHandler(Server &server, Client &client);

	~PassCommandHandler();

	void execute(const std::vector<std::string> &params);
};

/*
 * ========================================================================== *
 * USER
 * ========================================================================== *
 */

class UserCommandHandler : public AbstractCommandHandler
{
  public:
	UserCommandHandler(Server &server, Client &client);

	~UserCommandHandler();

	void execute(const std::vector<std::string> &params);
};

/*
 * ========================================================================== *
 * NICK
 * ========================================================================== *
 */

class NickCommandHandler : public AbstractCommandHandler
{
  public:
	NickCommandHandler(Server &server, Client &client);

	~NickCommandHandler();

	void execute(const std::vector<std::string> &params);
};

/*
 * ========================================================================== *
 * CAP
 * ========================================================================== *
 */

class CapCommandHandler : public AbstractCommandHandler
{
  public:
	CapCommandHandler(Server &server, Client &client);

	~CapCommandHandler();

	void execute(const std::vector<std::string> &params);
};

/*
 * ========================================================================== *
 * PING
 * ========================================================================== *
 */

class PingCommandHandler : public AbstractCommandHandler
{
  public:
	PingCommandHandler(Server &server, Client &client);

	~PingCommandHandler();

	void execute(const std::vector<std::string> &params);
};

/*
 * ========================================================================== *
 * QUIT
 * ========================================================================== *
 */

class QuitCommandHandler : public AbstractCommandHandler
{
  public:
	QuitCommandHandler(Server &server, Client &client);

	~QuitCommandHandler();

	void execute(const std::vector<std::string> &params);
};

/*
 * ========================================================================== *
 * CHANNEL
 * ========================================================================== *
 */

class AChannelCommandHandler : public AbstractCommandHandler
{
  protected:
	std::vector<std::string> parseChannelList(const std::string &channelList);

  public:
	AChannelCommandHandler(Server &server, Client &client);
	virtual ~AChannelCommandHandler();

	virtual void execute(const std::vector<std::string> &params) = 0;
};

/*
 * ========================================================================== *
 * LIST CHANNEL
 * ========================================================================== *
 */

 class ListChannelCommandHandler : public AChannelCommandHandler
 {
   public:
	ListChannelCommandHandler(Server &server, Client &client);

	~ListChannelCommandHandler();

	void execute(const std::vector<std::string> &params);
 };

/*
 * ========================================================================== *
 * JOIN
 * ========================================================================== *
 */

class JoinChannelCommandHandler : public AChannelCommandHandler
{
  public:
	JoinChannelCommandHandler(Server &server, Client &client);

	~JoinChannelCommandHandler();

	void execute(const std::vector<std::string> &params);
};

/*
 * ========================================================================== *
 * PART
 * ========================================================================== *
 */

class PartChannelCommandHandler : public AChannelCommandHandler
{
  public:
	PartChannelCommandHandler(Server &server, Client &client);

	~PartChannelCommandHandler();

	void execute(const std::vector<std::string> &params);
};

/*
 * ========================================================================== *
 * KICK
 * ========================================================================== *
 */

class KickChannelCommandHandler : public AChannelCommandHandler
{
  public:
	KickChannelCommandHandler(Server &server, Client &client);

	~KickChannelCommandHandler();

	void execute(const std::vector<std::string> &params);
};

/*
 * ========================================================================== *
 * TOPIC
 * ========================================================================== *
 */

class TopicCommandHandler : public AChannelCommandHandler
{
  public:
	TopicCommandHandler(Server &server, Client &client);

	~TopicCommandHandler();

	void execute(const std::vector<std::string> &params);
};

/*
 * ========================================================================== *
 * MODE
 * ========================================================================== *
 */

class ModeCommandHandler : public AChannelCommandHandler
{
  public:
	ModeCommandHandler(Server &server, Client &client);

	~ModeCommandHandler();

	void execute(const std::vector<std::string> &params);
};

/*
 * ========================================================================== *
 * INVITE
 * ========================================================================== *
 */

class InviteCommandHandler : public AChannelCommandHandler
{
  public:
	InviteCommandHandler(Server &server, Client &client);

	~InviteCommandHandler();

	void execute(const std::vector<std::string> &params);
};

/*`
 * ========================================================================== *
 * PRIVMSG
 * ========================================================================== *
 */

class PrivmsgCommandHandler : public AChannelCommandHandler
{
  public:
	PrivmsgCommandHandler(Server &server, Client &client);

	~PrivmsgCommandHandler();

	void execute(const std::vector<std::string> &params);
};

/*`
 * ========================================================================== *
 * WHOIS
 * ========================================================================== *
 */

class WhoisCommandHandler : public AbstractCommandHandler
{
  public:
	WhoisCommandHandler(Server &server, Client &client);

	~WhoisCommandHandler();

	void execute(const std::vector<std::string> &params);
};

#endif