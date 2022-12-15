#include "irc.hpp"

void	channelMode(Command &c, Server &s, Client *sender)
{
	Channel *chan = sender->getChannelByName(c.getArgs()[0]);
	if(!chan)
		sender->send_msg(ERR_NOTONCHANNEL(*s.getChannelByName(c.getArgs()[0]), sender->getNickname()));
	else if(sender != chan->getOperator())
		sender->send_msg(ERR_CHANOPRIVSNEEDED(*chan, sender->getNickname()));
	else
	{
		if(c.getArgs()[1] == "+i")
			chan->setInviteMode(true);
		else if (c.getArgs()[1] == "-i")
			chan->setInviteMode(false);
		else
			sender->send_msg(ERR_UMODEUNKNOWNFLAG(sender->getNickname(), c.getArgs()[1]));
	}
}

void	userMode(Command &c, Server &s, Client *sender)
{
	if (c.getArgs().size() < 2)
	{
		if (c.getArgs()[0] != sender->getNickname())
		{
			sender->send_msg(ERR_USERSDONTMATCH(sender->getNickname()));
		}
		else
		{
			sender->send_msg(RPL_UMODEIS(sender->getNickname(), sender->getIsOp() ? "+o" : ""));
		}
	}
	else if (sender->getIsOp())
	{
		if (c.getArgs()[1] == "+o")
			sender->setOp(true);
		else if (c.getArgs()[1] == "-o")
			sender->setOp(false);
		else {
			sender->send_msg(ERR_UMODEUNKNOWNFLAG(sender->getNickname(), c.getArgs()[1]));
			return ;
		}
		Client	*target = s.getClientsByNickname(c.getArgs()[0]);
		if (target != sender)
			sender->send_msg(":" + target->getNickname() + "!" + target->getUsername() + "@" + target->getServername() + " MODE " + target->getNickname() + " " + c.getArgs()[1]);
		target->send_msg(":" + target->getNickname() + "!" + target->getUsername() + "@" + target->getServername() + " MODE " + target->getNickname() + " " + c.getArgs()[1]);
	}
}

void	cmdMode(Command &c, Server &s)
{
	Client	*sender = c.getSender();
	if (!c.getSender()->getIsConnected())
	{
		return ;
	}
	if (c.getArgs().size() < 1)
	{
		sender->send_msg(ERR_NEEDMOREPARAMS(c, sender->getNickname()));
	}
	if (c.getArgs()[0][0] == '#')
	{
		if (s.getChannelByName(c.getArgs()[0]))
			channelMode(c, s, sender);
		else
			sender->send_msg(ERR_NOSUCHCHANNEL(c.getArgs()[0], sender->getNickname()));
	}
	else
		userMode(c, s, sender);
}
