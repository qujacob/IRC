#include "irc.hpp"

void	inviteUser(Client *target, Client *sender, Channel *channel)
{
	channel->getInvited().push_back(target);
	target->send_msg(":" + sender->getNickname() + "!" + sender->getUsername() + "@" + sender->getServername() + " INVITE " + target->getNickname() + " " + channel->getChannelName());
	sender->send_msg(":" + sender->getNickname() + "!" + sender->getUsername() + "@" + sender->getServername() + " INVITE " + target->getNickname() + " " + channel->getChannelName());
	sender->send_msg(RPL_INVITING(sender->getNickname(), channel->getChannelName(), target->getNickname()));
}

void	cmdInvite(Command &c, Server &s)
{
	Client	*sender = c.getSender();
	Client	*target = s.getClientsByNickname(c.getArgs()[0]);
	Channel	*channel = sender->getChannelByName(c.getArgs()[1]);
	if (!c.getSender()->getIsConnected())
	{
		return ;
	}
	if (c.getArgs().size() < 2)
	{
		sender->send_msg(ERR_NEEDMOREPARAMS(c, sender->getNickname()));
		return ;
	}
	if (!target)
		sender->send_msg(ERR_NOSUCHNICK(c.getArgs()[0], sender->getNickname()));
	else if (!channel)
	{
		if (!s.getChannelByName(c.getArgs()[1]))
			sender->send_msg(ERR_NOSUCHNICK(c.getArgs()[1], sender->getNickname()));
		else
			sender->send_msg(ERR_NOTONCHANNEL(*s.getChannelByName(c.getArgs()[1]), sender->getNickname()));
	}
	else if (target->getChannelByName(c.getArgs()[1]))
		sender->send_msg(ERR_USERONCHANNEL(*target, *channel, sender->getNickname()));
	else if (sender != channel->getOperator())
		sender->send_msg(ERR_CHANOPRIVSNEEDED(*channel, sender->getNickname()));
	else
		inviteUser(target, sender, channel);

}
