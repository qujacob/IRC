#include "irc.hpp"

/*
	COMMAND RESPONSES
*/

// remplacer le deuxieme "c._nickname" par c._user et "localhost" par s._host
std::string	RPL_WELCOME(Client c) {
	return ("001 " + c.getNickname() + " :Welcome to the Internet Relay Network " + c.getNickname() + "!" + c.getUsername() + "@" + c.getServername());
	// return (":" + c.getNickname() + " :Welcome to the Internet Relay Network " + c.getNickname() + "!" + c.getUsername() + "@" + c.getServername());
}

std::string	RPL_YOURHOST(Server &s, std::string nick) {
	return ("002 " + nick + " Your host is " + s.getServerName() + ", running version " + s.getVersion());
}

std::string	RPL_CREATED(Server &s, std::string nick) {
	return ("003 " + nick + " This server was created " + s.getDate());
}

std::string	RPL_MYINFO(Server s, std::string nick) {
	return ("004 " + nick + " " + s.getServerName() + " " + s.getVersion() + " " + "<inserer s._user_modes>" + " " + "<inserer s._chan_modes>");
}

std::string RPL_BOUNCE(Server s, std::string nick) {
	return ("005 " + nick + " :Try server " + s.getServerName() + ", port " + s.getPort());
}

/*std::string RPL_LUSERCLIENT(Server s)
{
	return("251 :There are " + s.getNbConnection() " users and " +  <integer> + " invisible on " +  <integer> " servers");
} -> need nombre d'users invisible et nombre de servers*/

/*std::string RPL_LUSEROP(Server s)
{
	return("252 <integer> :operator(s) online")
} -> need nombre d'users qui sont operators*/

// std::string	RPL_TRACESERVICE() {}

// std::string	RPL_TRACECLASS() {}

// std::string	RPL_SERVLIST() {}

// std::string	RPL_SERVLISTEND() {}

// std::string	RPL_TRACEEND() {}

// std::string	RPL_TRYAGAIN() {}

// std::string	RPL_INVITELIST() {}

// std::string	RPL_ENDOFINVITELIST() {}

// std::string	RPL_EXCEPTLIST() {}

// std::string	RPL_ENDOFEXCEPTLIST() {}

// std::string	RPL_YOURESERVICE() {}

std::string	RPL_UMODEIS(std::string nick, std::string mode) {
	return("221 " + nick + " :" + mode);
}

std::string	RPL_INVITING(std::string nick, std::string channel, std::string target) {
	return ("341 " + nick + " " + channel + " " + target);
}

std::string	RPL_YOUREOPER(std::string str) {
	return("381 " + str + " :You are now an IRC Operator");
}

std::string RPL_WHOREPLY(std::string nick, std::string host, std::string server, std::string real, Command &cmd)
{
	return("352 " + cmd.getSender()->getNickname() + " " + nick + " " + host + " " + server + " H " + real);
} 

std::string RPL_ENDOFWHO(Client &c, std::string users)
{
	return("315 " + c.getNickname() + " " + users + ":End of /WHO list.");
}

/*
	ERROR REPLIES
*/

std::string ERR_NOSUCHNICK(std::string str, std::string nick)
{
	return("401 " + nick + " " + str + " :No such nick/channel");
}

std::string ERR_NOSUCHSERVER(std::string str, std::string nick)
{
	return("402 " + nick + " " + str + " :No such server");
}

std::string ERR_NOSUCHCHANNEL(std::string channel, std::string nick)
{
	return("403 " + nick + " " + channel + " :No such channel");
}

std::string ERR_CANNOTSENDTOCHAN(std::string channel, std::string nick)
{
	return("404 " + nick + " " + channel + " :Cannot send to channel");
}

std::string ERR_TOOMANYCHANNELS(std::string channel, std::string nick)
{
	return("405 " + nick + " " + channel + " :You have joined too many channels");
}

std::string ERR_WASNOSUCHNICK(Client c, std::string nick)
{
	return("406 " + nick + " " + c.getNickname() + " :There was no such nickname");
}

/*std::string ERR_TOOMANYTARGETS()
{
	return("<target> :Duplicate recipients. No message 
                         delivered");
}*/

std::string ERR_NOORIGIN(std::string nick)
{
	return("409 " + nick + " :No origin specified");
}

std::string ERR_NORECIPIENT(Command cmd, std::string nick)
{
	return("411 " + nick + " :No recipient given (" + cmd.getCmd() + ")");
}

std::string ERR_NOTEXTTOSEND(std::string nick)
{
	return("412 " + nick + " :No text to send");
}

std::string ERR_UNKNOWNCOMMAND(Command cmd, std::string nick)
{
	return("421 " + nick + " " + cmd.getCmd() + " :Unknown command");
}

std::string ERR_NONICKNAMEGIVEN(std::string nick)
{
	return("431 " + nick + " :No nickname given");
}

std::string ERR_ERRONEUSNICKNAME(std::string nick, std::string str)
{
	return ("432 " + nick + " " + str + " :Erroneus nickname");
}

std::string ERR_NICKNAMEINUSE(Client c, std::string nick)
{
	return("433 " + c.getNickname() + " " + nick + " :Nickname is already in use");
}

std::string ERR_USERNOTINCHANNEL(std::string nick, std::string target, std::string channel)
{
	return("441 " + nick + " " + target + " " + channel + " :They aren't on that channel");
}

std::string ERR_NOTONCHANNEL(Channel cl, std::string nick)
{
	return("442 " + nick + " " + cl.getChannelName() + " :You're not on that channel");
}

std::string ERR_USERONCHANNEL(Client c, Channel cl, std::string nick)
{
	return("443 " + c.getNickname() + " " + nick + " " + cl.getChannelName() + " :is already on channel");
}

std::string ERR_NOLOGIN(Client c)
{
	return("444 " + c.getUsername() + " :User not logged in");
}

std::string ERR_NEEDMOREPARAMS(Command cmd, std::string nick)
{
	return("461 " + nick + " " + cmd.getCmd() + " :Not enough parameters");
}

std::string ERR_ALREADYREGISTERED(std::string nick)
{
	return("462 " + nick + ":Unauthorized command (already registered)");
}

std::string ERR_PASSWDMISMATCH(std::string nick)
{
	return("464 " + nick + ":Password incorrect");
}

std::string ERR_YOUREBANNEDCREEP(std::string nick)
{
	return("465 " + nick + " :You are banned from this server");
}

std::string ERR_KEYSET(Channel cl, std::string nick)
{
	return("467 " + nick + " " + cl.getChannelName() + " :Channel key already set");
}

std::string ERR_CHANNELISFULL(Channel cl, std::string nick)
{
	return("471 " + nick + " " + cl.getChannelName() + " :Cannot join channel (+l)");
}

/*std::string ERR_UNKNOWNMODE()
{
	return("472 " + input ? + ":is unknown mode cERR_NOCHANMODEShar to me")
}*/

std::string ERR_INVITEONLYCHAN(std::string nick, std::string channel)
{
	return("473 " + nick + " " + channel + " :Cannot join channel (+i)");
}

std::string ERR_BANNEDFROMCHAN(Channel cl, std::string nick)
{
	return("474 " + nick + " " + cl.getChannelName() + " :Cannot join channel (+b)");
}

std::string ERR_BADCHANNELKEY(Channel cl, std::string nick)
{
	return("475 " + nick + " " + cl.getChannelName() + " :Cannot join channel (+k)");
}

std::string ERR_NOPRIVILEGES(std::string nick)
{
	return("481 " + nick + " :Permission Denied- You're not an IRC operator");
}

std::string ERR_CHANOPRIVSNEEDED(Channel cl, std::string nick)
{
	return("482 " + nick + " " + cl.getChannelName() + " :You're not channel operator");
}

std::string ERR_UMODEUNKNOWNFLAG(std::string nick, std::string mode)
{
	return("501 " + nick + " :" + mode + " Unknown MODE flag");
}

std::string ERR_USERSDONTMATCH(std::string nick)
{
	return("502 " + nick + " :Cannot change mode for other users");
}


//  std::string	ERR_NOSUCHSERVICE() {}

// std::string	ERR_BADMASK() {}

// std::string	ERR_UNAVAILRESOURCE() {}

// std::string	ERR_BADCHANMASK() {}

// std::string	ERR_NOCHANMODES() {}

// std::string	ERR_BANLISTFULL() {}

// std::string	ERR_RESTRICTED() {}

// std::string	ERR_UNIQOPRIVSNEEDED() {}


