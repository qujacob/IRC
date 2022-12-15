#include "irc.hpp"

#include <sys/types.h>
#include <sys/socket.h>

// Besoin de recup le client par son username
// Faire une map qui permettrait d'avoir une cle (username en string) et la valeur serait le client 

void    msgClient(Client *target, Client *sender, Command &c)
{
	std::string msg;
	for(size_t i = 1; i < c.getArgs().size(); i++)
	{
		if (i != 1)
			msg += ' ';
		msg += c.getArgs()[i]; 
	}
	if (msg[0] == ':')
		msg.erase(0, 1);

	target->send_msg(":" + sender->getNickname() + "!" + sender->getUsername() + "@" + sender->getServername() + " PRIVMSG " + target->getNickname() + " :" + msg);
}

void    msgChannel(Channel *ch_target, Client *sender, Command &c)
{
	std::string msg;
	for(size_t i = 1; i < c.getArgs().size(); i++)
	{
		if (i != 1)
			msg += ' ';
		msg += c.getArgs()[i]; 
	}
	if (msg[0] == ':')
		msg.erase(0, 1);
	else
	{
		for (size_t i = 0; i < ch_target->getClients().size(); i++)
		{
			if (ch_target->getClients()[i] != sender)
				ch_target->getClients()[i]->send_msg(":" + sender->getNickname() + "!" + sender->getUsername() + "@" + sender->getServername() + " PRIVMSG " + ch_target->getChannelName() + " :" + msg);
		}
	}
}

void	cmdPrivMsg(Command &c, Server &s) 
{
	Client  *sender = c.getSender();
	if (!c.getSender()->getIsConnected())
	{
		return ;
	}
	if (c.getArgs().size() < 2)
	{
		sender->send_msg(ERR_NEEDMOREPARAMS(c, sender->getNickname()));
		return ;
	}

	Client  *target = s.getClientsByNickname(c.getArgs()[0]);
	if (target)
		msgClient(target, sender, c);
	else
	{
		Channel *ch_target = s.getChannelByName(c.getArgs()[0]);
		Channel *ch_client = sender->getChannelByName(c.getArgs()[0]);
		if (ch_client)
			msgChannel(ch_client, sender, c);
		else if (ch_target)
			sender->send_msg(ERR_CANNOTSENDTOCHAN(c.getArgs()[0], sender->getNickname()));
		else
			sender->send_msg(ERR_NOSUCHNICK(c.getArgs()[0], sender->getNickname()));
	}
}
