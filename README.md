*This project has been created as part of the 42 curriculum by kroyo-di, alejhern.*

# ft_irc

## Description

An IRC server built from scratch in C++98, using non-blocking sockets and a single `poll()` call. It supports authentication, nicknames, channels, private messages, and channel operator commands. Tested with `nc` and with **irssi** as our reference client.

## Mandatory part

* C++98, compiles cleanly with `-Wall -Wextra -Werror`.
* Handles multiple clients at once, never blocks, never forks.
* A single `poll()` handles every read, write, and accept.
* Works with a real IRC client over TCP/IPv4, without errors.
* Supports: authentication, nickname/username, joining channels, private messages (user and channel), channel operators vs regular users, and the operator-only commands (`KICK`, `INVITE`, `TOPIC`, `MODE`).

## Our IRC commands

* **PASS** — sets the connection password.
* **NICK** — sets or changes the nickname.
* **USER** — sets username and real name at registration.
* **JOIN** — joins a channel (creates it, as operator, if it doesn't exist).
* **PART** — leaves one or more channels.
* **PRIVMSG** — sends a message to a user or a channel.
* **TOPIC** — views or changes a channel's topic.
* **KICK** — removes a member from a channel (operator only).
* **INVITE** — invites a user to a channel.
* **MODE** — changes channel mode (operator only): `i` invite-only, `t` topic restricted to operators, `k` channel key, `o` give/take operator, `l` user limit.
* **QUIT** — disconnects, with an optional reason.
* **CAP** — capability negotiation, for client compatibility (e.g. irssi).
* **PING** — replies to keep-alive checks.
* **LIST** — lists active channels, with user count and topic.
* **WHOIS** — info about a given user.
* **WHO** — lists a channel's members with extra detail.

## Instructions

**Compile:**
```
make
```
(also `make clean`, `make fclean`, `make re`)

**Run:**
```
./ircserv <port> <password>
```

**Connect:**
```
nc <IP> <PORT>
```
or with an IRC client:
```
irssi
/connect <IP> <PORT> <password>
```

## Resources

* [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/html/)
* [RFC 1459 — Internet Relay Chat Protocol](https://datatracker.ietf.org/doc/html/rfc1459)
* [RFC 2812 — Internet Relay Chat: Client Protocol](https://datatracker.ietf.org/doc/html/rfc2812)
* `man poll`, `man fcntl`, `man socket`

**How AI was used:** AI was used as a pair-programming aid — setting up sockets/`poll()`/signals correctly, reviewing code to catch bugs (a use-after-free on `QUIT`, a crash from unhandled `SIGPIPE`, malformed IRC replies, channels not cleaned up after disconnects), clarifying RFC behavior, and designing test scripts (raw sockets, `irssi`, `valgrind`) to validate edge cases from the subject. All suggestions were reviewed and tested before being kept.

## Contributors

* [alejhern](https://profile.intra.42.fr/users/alejhern)
* [kroyo-di](https://profile.intra.42.fr/users/kroyo-di)