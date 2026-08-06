*This project has been created as part of the 42 curriculum by kroyo-di, alejhern.*

# ft_irc

##  Description

This project is about creating our own IRC (Internet Relay Chat) server from scratch, in C++98, using only low-level sockets and a single `poll()` call. IRC is a text-based protocol for real-time messaging, created in 1988, still widely used for group discussions in channels as well as private messages between users.

We built a non-blocking, single-threaded server able to handle several clients at the same time, and we tested it both with raw TCP tools (like `nc`) and with a real IRC client (`irssi`), used as our reference client throughout development.

## 📋 Mandatory part

According to the subject, our server must:

* Be developed in C++98, compiling cleanly with `-Wall -Wextra -Werror`.
* Handle multiple clients at the same time, without ever hanging or blocking.
* Use a single `poll()` (or equivalent) call to manage every I/O operation: accepting new connections, reading, and writing.
* Never fork, and keep every file descriptor non-blocking.
* Accept connections from a real IRC client without any error, over TCP/IP (v4).
* Behave, from the client's point of view, like any other IRC server, for the following features:
  * Authentication with a server password, setting a nickname and a username, and joining a channel.
  * Sending and receiving private messages, either to a single user or to a channel.
  * Forwarding every message sent to a channel to all the other clients who joined it.
  * Distinguishing between channel operators and regular users.
  * Implementing the commands reserved to channel operators (`KICK`, `INVITE`, `TOPIC`, `MODE`).

## 🔑 Our IRC commands

These are the commands currently implemented by our server:

* **PASS** — sets the connection password required before registering.
* **NICK** — gives the client a nickname, or changes an existing one.
* **USER** — used once, at registration, to set the username and real name of the client.
* **JOIN** — makes the client join a channel, creating it (and becoming its operator) if it does not exist yet.
* **PART** — makes the client leave one or several channels.
* **PRIVMSG** — sends a private message, either to another user or to every member of a channel.
* **TOPIC** — views or changes the topic of a channel.
* **KICK** — forces a member out of a channel (operators only).
* **INVITE** — invites a client to join a channel, required to bypass invite-only (`+i`) mode.
* **MODE** — changes a channel's mode (operators only). Supported flags:
  * `i` — invite-only channel.
  * `t` — restricts the `TOPIC` command to channel operators.
  * `k` — sets or removes the channel key (password).
  * `o` — gives or takes channel operator privilege.
  * `l` — sets or removes the channel's user limit.
* **QUIT** — disconnects the client, with an optional reason shared with every channel they were part of.
* **CAP** — handles the capability negotiation some clients (like `irssi`) perform at connection time.
* **PING** — replies to keep-alive checks sent by the client, avoiding unwanted disconnections.
* **LIST** — lists every channel currently active on the server, along with its user count and topic.
* **WHOIS** — shows information about a given user (username, host, channels they are in...).
* **WHO** — lists the members of a given channel, with more detail than `NAMES` (host, server, operator status...).

## ⚙️ Instructions

### Compile

```
make
```

Other available rules: `make clean`, `make fclean`, `make re`.

### Start the server

```
./ircserv <port> <password>
```

* `port`: the port the server will listen on for incoming IRC connections.
* `password`: the connection password, required from any client that wants to connect.

### Connect to the server

With a raw TCP tool, useful for quick, manual protocol testing:

```
nc <IP ADDRESS> <PORT>
```

Or with a real IRC client. We used **irssi** as our reference client:

```
irssi
/connect <IP ADDRESS> <PORT> <password>
```

## 👨‍💻 Resources

* [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/html/)
* [RFC 1459 — Internet Relay Chat Protocol](https://datatracker.ietf.org/doc/html/rfc1459)
* [RFC 2812 — Internet Relay Chat: Client Protocol](https://datatracker.ietf.org/doc/html/rfc2812)
* [Basic IRC Commands](https://www.mirc.com/help/html/index.html?basic_irc_commands.html)
* `man poll`, `man fcntl`, `man socket` — the manual pages for every system call used in this project.
* [irssi documentation](https://irssi.org/documentation/) — used to understand the behavior of our reference client during testing.

### How AI was used

We used Claude (Anthropic) as a learning and pair-programming aid throughout the project, always reviewing and testing every suggestion ourselves before integrating it. It was used for:

* Getting the initial socket/`poll()`/signal-handling setup right, and understanding *why* each system call and flag was needed, not just copying code.
* Reviewing our code across the project's lifetime to catch bugs before they became hard to trace: a use-after-free on `QUIT`, a server crash caused by an unhandled `SIGPIPE`, incorrect IRC reply formatting (missing/duplicated prefixes), and channels not being cleaned up correctly after abrupt disconnections.
* Explaining relevant sections of the IRC RFCs when our own reading left us unsure of the expected behavior (e.g. numeric reply formats, registration order).
* Designing and running test scripts (via raw sockets and via `irssi`, including memory checks with `valgrind`) to validate our implementation against edge cases described in the subject, such as fragmented commands, abrupt client disconnections, and a slow/stalled client being flooded by another.

Every piece of code suggested was reviewed, tested, and understood by the team before being kept in the final project.

## 💪 Contributors

[kroyo-di](https://profile.intra.42.fr/users/kroyo-di)

[alejhern](https://profile.intra.42.fr/users/alejhern)