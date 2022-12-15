#include "irc.hpp"

void	kickUsers(Channel *channel, Client *sender, Command &c)
{
	std::string			name;
	std::stringstream 	input_stringstream(c.getArgs()[1]);
	while (std::getline(input_stringstream, name, ','))
	{
		Client	*target = channel->getClientByName(name);
		if (sender != channel->getOperator())
			sender->send_msg(ERR_CHANOPRIVSNEEDED(*channel, sender->getNickname()));
		else if (!target)
			sender->send_msg(ERR_USERNOTINCHANNEL(sender->getNickname(), name, channel->getChannelName()));
		else
		{
			std::string msg = c.getArgs().size() > 2 ? "" : "Kicked";
			for(size_t i = 2; i < c.getArgs().size(); i++)
			{
				if (i != 2)
					msg += ' ';
				msg += c.getArgs()[i]; 
			}
			if (msg[0] == ':')
				msg.erase(0, 1);

			sender->send_msg(":" + sender->getNickname() + "!" + sender->getUsername() + "@" + sender->getServername() + " KICK " + channel->getChannelName() + " " + target->getNickname() + " :" + msg);
			channel->send_msg(":" + sender->getNickname() + "!" + sender->getUsername() + "@" + sender->getServername() + " KICK " + channel->getChannelName() + " " + target->getNickname() + " :" + msg, *sender);
			target->send_msg(":" + sender->getNickname() + "!" + sender->getUsername() + "@" + sender->getServername() + " KICK " + channel->getChannelName() + " " + target->getNickname() + " :" + msg);

			channel->eraseClient(target);
			target->eraseChannel(channel);
		}
	}
}

void	cmdKick(Command &c, Server &s)
{
	Client	*sender = c.getSender();
	if (!c.getSender()->getIsConnected())
	{
		return ;
	}
	if (c.getArgs().size() < 2)
		sender->send_msg(ERR_NEEDMOREPARAMS(c, sender->getNickname()));
	else
	{
		std::string			name;
		std::stringstream 	input_stringstream(c.getArgs()[0]);
		while (std::getline(input_stringstream, name, ','))
		{
			Channel	*channel = sender->getChannelByName(name);
			if (channel)
			{
				kickUsers(channel, sender, c);
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
}
