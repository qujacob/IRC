#pragma once

#include "irc.hpp"

class Client;
class Channel;
class Command;
class Server;

/*
	<message>  ::= [':' <prefix> <SPACE> ] <command> <params> <crlf>
	<prefix>   ::= <servername> | <nick> [ '!' <user> ] [ '@' <host> ]
	<command>  ::= <letter> { <letter> } | <number> <number> <number>
	<SPACE>    ::= ' ' { ' ' }
	<params>   ::= <SPACE> [ ':' <trailing> | <middle> <params> ]

	<middle>   ::= <Any *non-empty* sequence of octets not including SPACE
				or NUL or CR or LF, the first of which may not be ':'>
	<trailing> ::= <Any, possibly *empty*, sequence of octets not including
					NUL or CR or LF>

	<crlf>     ::= CR LF
	https://www.rfc-editor.org/rfc/rfc1459.html#section-2.3.1
	https://modern.ircdocs.horse/#rplwelcome-001
*/

std::string	RPL_WELCOME(Client c);

std::string	RPL_YOURHOST(Server &s, std::string nick);
std::string	RPL_CREATED(Server &s, std::string nick);
std::string	RPL_MYINFO(Server s, std::string nick);
std::string RPL_BOUNCE(Server s, std::string nick);
std::string	RPL_UMODEIS(std::string nick, std::string mode);
std::string	RPL_INVITING(std::string nick, std::string channel, std::string target);
std::string	RPL_YOUREOPER(std::string str);
std::string RPL_WHOREPLY(std::string nick, std::string host, std::string server, std::string real, Command &cmd);
std::string RPL_ENDOFWHO(Client &c, std::string users);

std::string ERR_NOSUCHNICK(std::string str, std::string nick);
std::string ERR_NOSUCHSERVER(std::string str, std::string nick);
std::string ERR_NOSUCHCHANNEL(std::string channel, std::string nick);
std::string ERR_CANNOTSENDTOCHAN(std::string channel, std::string nick);
std::string ERR_TOOMANYCHANNELS(std::string channel, std::string nick);
std::string ERR_WASNOSUCHNICK(Client c, std::string nick);
std::string ERR_NOORIGIN(std::string nick);
std::string ERR_NORECIPIENT(Command cmd, std::string nick);
std::string ERR_NOTEXTTOSEND(std::string nick);
std::string ERR_UNKNOWNCOMMAND(Command cmd, std::string nick);
std::string ERR_NONICKNAMEGIVEN(std::string nick);
std::string ERR_ERRONEUSNICKNAME(std::string nick, std::string str);
std::string ERR_NICKNAMEINUSE(Client c, std::string nick);
std::string ERR_USERNOTINCHANNEL(std::string nick, std::string target, std::string channel);
std::string ERR_NOTONCHANNEL(Channel cl, std::string nick);
std::string ERR_USERONCHANNEL(Client c, Channel cl, std::string nick);
std::string ERR_NOLOGIN(Client c, std::string nick);
std::string ERR_NEEDMOREPARAMS(Command cmd, std::string nick);
std::string ERR_PASSWDMISMATCH(std::string str);
std::string ERR_YOUREBANNEDCREEP(std::string nick);
std::string ERR_ALREADYREGISTERED(std::string nick);
std::string ERR_NOPRIVILEGES(std::string nick);

std::string ERR_KEYSET(Channel cl, std::string nick);

std::string ERR_CHANNELISFULL(Channel cl, std::string nick);
std::string ERR_INVITEONLYCHAN(std::string nick, std::string channel);
std::string ERR_BANNEDFROMCHAN(Channel cl, std::string nick);
std::string ERR_BADCHANNELKEY(Channel cl, std::string nick);
std::string ERR_CHANOPRIVSNEEDED(Channel cl, std::string nick);
std::string ERR_UMODEUNKNOWNFLAG(std::string nick, std::string mode);
std::string ERR_USERSDONTMATCH(std::string nick);
