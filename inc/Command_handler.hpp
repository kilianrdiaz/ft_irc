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
#define COMMAND_HANDLER_HPP

#include <exception>
#include <string>
#include <vector>

#include "Client.hpp"
#include "Server.hpp"
#include "command_excepts.hpp"

typedef void (*HandlerExecutor)(
    Server &,
    Client &,
    const std::vector<std::string> &
);

struct Command
{
    std::string name;
    std::vector<std::string> params;
};

class AbstractCommandHandler
{
    private:
        AbstractCommandHandler(
            const AbstractCommandHandler &
        );

        AbstractCommandHandler &operator=(
            const AbstractCommandHandler &
        );

    protected:
        Server &_server;
        Client &_client;

    public:
        AbstractCommandHandler(
            Server &server,
            Client &client
        );

        virtual ~AbstractCommandHandler();

        void setCommandName(
            const std::string commandName
        );

        const std::string &getCommandName() const;

        static Command parseLine(const std::string &line);

        static void executeCommand(
            Server &server,
            Client &client,
            const std::string commandName,
            const std::vector<std::string> &params
        );

        virtual void execute(
            const std::vector<std::string> &params
        ) = 0;
};


/*
 * ========================================================================== *
 * PASS
 * ========================================================================== *
 */

class PassCommandHandler : public AbstractCommandHandler
{
    public:
        PassCommandHandler(
            Server &server,
            Client &client
        );

        virtual ~PassCommandHandler();

        virtual void execute(
            const std::vector<std::string> &params
        );
};


/*
 * ========================================================================== *
 * USER
 * ========================================================================== *
 */

class UserCommandHandler : public AbstractCommandHandler
{
    public:
        UserCommandHandler(
            Server &server,
            Client &client
        );

        virtual ~UserCommandHandler();

        virtual void execute(
            const std::vector<std::string> &params
        );
};


/*
 * ========================================================================== *
 * NICK
 * ========================================================================== *
 */

class NickCommandHandler : public AbstractCommandHandler
{
    public:
        NickCommandHandler(
            Server &server,
            Client &client
        );

        virtual ~NickCommandHandler();

        virtual void execute(
            const std::vector<std::string> &params
        );
};

/*
 * ========================================================================== *
 * CAP
 * ========================================================================== *
 */

class CapCommandHandler : public AbstractCommandHandler
{
    public:
        CapCommandHandler(
            Server &server,
            Client &client
        );

        virtual ~CapCommandHandler();

        virtual void execute(
            const std::vector<std::string> &params
        );
};

/*
 * ========================================================================== *
 * PING
 * ========================================================================== *
 */

class PingCommandHandler : public AbstractCommandHandler
{
    public:
        PingCommandHandler(
            Server &server,
            Client &client
        );

        virtual ~PingCommandHandler();

        virtual void execute(
            const std::vector<std::string> &params
        );
};


/*
 * ========================================================================== *
 * QUIT
 * ========================================================================== *
 */

// class QuitCommandHandler : public AbstractCommandHandler
// {
//     public:
//         QuitCommandHandler(
//             Server &server,
//             Client &client
//         );

//         virtual ~QuitCommandHandler();

//         virtual void execute(
//             const std::vector<std::string> &params
//         );
// };


/*
 * ========================================================================== *
 * PRIVMSG
 * ========================================================================== *
 */

// class PrivmsgCommandHandler : public AbstractCommandHandler
// {
//     public:
//         PrivmsgCommandHandler(
//             Server &server,
//             Client &client
//         );

//         virtual ~PrivmsgCommandHandler();

//         virtual void execute(
//             const std::vector<std::string> &params
//         );
// };

/*
 * ========================================================================== *
 * CHANNEL
 * ========================================================================== *
 */

class AChannelCommandHandler : public AbstractCommandHandler
{
    public:
        AChannelCommandHandler(
            Server &server,
            Client &client
        );
    protected:
        std::vector<std::string> parseChannelList(
            const std::string &channelList
        );

    public:
        virtual ~AChannelCommandHandler();

        virtual void execute(
            const std::vector<std::string> &params
        ) = 0;
};


/*
 * ========================================================================== *
 * JOIN
 * ========================================================================== *
 */

class JoinChannelCommandHandler : public AChannelCommandHandler
{
    public:
        JoinChannelCommandHandler(
            Server &server,
            Client &client
        );

        virtual ~JoinChannelCommandHandler();

        virtual void execute(
            const std::vector<std::string> &params
        );
};


/*
 * ========================================================================== *
 * PART
 * ========================================================================== *
 */

class PartChannelCommandHandler : public AChannelCommandHandler
{
    public:
        PartChannelCommandHandler(
            Server &server,
            Client &client
        );

        virtual ~PartChannelCommandHandler();

        virtual void execute(
            const std::vector<std::string> &params
        );
};


/*
 * ========================================================================== *
 * KICK
 * ========================================================================== *
 */

class KickChannelCommandHandler : public AChannelCommandHandler
{
    public:
        KickChannelCommandHandler(
            Server &server,
            Client &client
        );

        virtual ~KickChannelCommandHandler();

        virtual void execute(
            const std::vector<std::string> &params
        );
};


/*
 * ========================================================================== *
 * TOPIC
 * ========================================================================== *
 */

class TopicCommandHandler : public AChannelCommandHandler
{
    public:
        TopicCommandHandler(
            Server &server,
            Client &client
        );

        virtual ~TopicCommandHandler();

        virtual void execute(
            const std::vector<std::string> &params
        );
};


/*
 * ========================================================================== *
 * MODE
 * ========================================================================== *
 */

class ModeCommandHandler : public AChannelCommandHandler
{
    public:
        ModeCommandHandler(
            Server &server,
            Client &client
        );

        virtual ~ModeCommandHandler();

        virtual void execute(
            const std::vector<std::string> &params
        );
};

#endif