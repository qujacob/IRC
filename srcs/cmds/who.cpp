#include "irc.hpp"

/*This command is used to query a list of users who match the provided mask. The server will answer this command with zero, one or more RPL_WHOREPLY, and end the list with RPL_ENDOFWHO.

The mask can be one of the following:

A channel name, in which case the channel members are listed.
An exact nickname, in which case a single user is returned.
A mask pattern, in which case all visible users whose nickname matches are listed. Servers MAY match other user-specific values, such as the hostname, server, real name or username. 
Servers MAY not support mask patterns and return an empty list.
Visible users are users who aren’t invisible (user mode +i) and who don’t have a common channel with the requesting client. Servers MAY filter or limit visible users replies arbitrarily.*/

void		whosInChan(Channel *cl, Client *sender, Command &c)
{
	std::string users;
	std::vector<Client *>::iterator begin = cl->getClients().begin();
	std::vector<Client *>::iterator end = cl->getClients().end();

	while (begin != end)
	{
		Client *client = *begin;
		users += client->getNickname() + " ";
		std::cout << client->getNickname() << std::endl;
		sender->send_msg(RPL_WHOREPLY(client->getNickname(), client->getHostname(), client->getServername(), client->getRealname(), c));
		begin++;
	}
	sender->send_msg(RPL_ENDOFWHO(*sender, users));
}

void	cmdWho(Command &c, Server &s) 
{
	(void)c,(void)s;
	Client	*sender = c.getSender();
	if (!c.getSender()->getIsConnected())
	{
		return ;
	}
    if (c.getArgs().size() < 1)
	{
		sender->send_msg(ERR_NEEDMOREPARAMS(c, sender->getNickname()));
		return ;
	}
	Client  *target = s.getClientsByNickname(c.getArgs()[0]);
	if (target)
	{
		sender->send_msg(RPL_WHOREPLY(target->getNickname(), target->getHostname(), target->getServername(), target->getRealname(), c));
		sender->send_msg(RPL_ENDOFWHO(*sender, target->getNickname()));
	}
	else
	{
		Channel *ch_target = s.getChannelByName(c.getArgs()[0]);
		if (ch_target)
		{
			whosInChan(ch_target, sender, c);
		}
		else
			sender->send_msg(ERR_NOSUCHNICK(c.getArgs()[0], sender->getNickname()));
	}
}
