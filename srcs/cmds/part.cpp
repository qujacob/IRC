#include "irc.hpp"

void	leaveChannel(Channel *channel, Client *sender, Command &c, Server &s)
{
	std::string msg = c.getArgs().size() > 1 ? "" : "Leaving";
	for(size_t i = 1; i < c.getArgs().size(); i++)
	{
		if (i != 1)
			msg += ' ';
		msg += c.getArgs()[i]; 
	}
	if (msg[0] == ':')
		msg.erase(0, 1);
	sender->send_msg(":" + sender->getNickname() + "!" + sender->getUsername() + "@" + sender->getServername() + " PART " + channel->getChannelName() + " :" + msg);
	channel->send_msg(":" + sender->getNickname() + "!" + sender->getUsername() + "@" + sender->getServername() + " PART " + channel->getChannelName() + " :" + msg, *sender);
	channel->eraseClient(sender);
	sender->eraseChannel(channel);
	if (!channel->getClients().size())
	{
		s.getChannels().erase(channel->getChannelName());
		delete channel;
	}
	else if (sender == channel->getOperator())
		channel->setOperator(channel->getClients().front());
}

void	cmdPart(Command &c, Server &s)
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
	std::string			name;
	std::stringstream 	input_stringstream(c.getArgs()[0]);
	while (std::getline(input_stringstream, name, ','))
	{
		Channel	*channel = sender->getChannelByName(name);
		if (channel)
		{
			leaveChannel(channel, sender, c, s);
		}
		else
		{
			Channel	*server_chann = s.getChannelByName(name);
			if (server_chann)
				sender->send_msg(ERR_NOTONCHANNEL(name, sender->getNickname()));
			else
				sender->send_msg(ERR_NOSUCHCHANNEL(name, sender->getNickname()));
		}
	}
}
