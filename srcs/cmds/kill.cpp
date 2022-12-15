#include "irc.hpp"

void	cmdKill(Command &c, Server &s) {
	if (!c.getSender()->getIsConnected())
	{
		return ;
	}
	if (!c.getSender()->getIsOp())
	{
		c.getSender()->send_msg(ERR_NOPRIVILEGES(c.getSender()->getNickname()));
	}
	else if (c.getArgs().size() < 2)
	{
		c.getSender()->send_msg(ERR_NEEDMOREPARAMS(c, c.getSender()->getNickname()));
	}
	else {
		Client	*target = s.getClientsByNickname(c.getArgs()[0]);

		if (target) {
			std::string	msg = "";
	
			for (size_t i = 1; i < c.getArgs().size(); i++)
				msg += c.getArgs()[i] + " ";
			target->send_msg(":" + c.getSender()->getNickname() + " kill :" + msg);

			for (size_t i = 0; i < s.getVctPfd().size(); i++)
			{
				if (s.getVctPfd()[i].fd == target->getFd())
				{
					target->setIsAlive(false);	
					break;
				}
			}
		}
		else
			c.getSender()->send_msg(ERR_NOSUCHNICK(c.getArgs()[0], c.getSender()->getNickname()));
	}
}
