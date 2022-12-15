#include "irc.hpp"

bool	checkNickname(std::string &str) {
	const char	unvalid[11] = "-[]\\`^{}_ ";
	if (str.find_first_of(unvalid) != std::string::npos || str.length() < 3 || str.length() > 20)
		return (0);
	return (1);
}

void	cmdNick(Command &c, Server &s) {
	(void)s;
	if (!c.getSender()->getPsStatus())
	{
		return ;
	}
	if (c.getArgs().size() != 1)
		c.getSender()->send_msg(ERR_NONICKNAMEGIVEN(c.getSender()->getNickname()));
	else if (s.getClientsByNickname(c.getArgs()[0]))
	{
		c.getSender()->send_msg(ERR_NICKNAMEINUSE(*c.getSender(), c.getArgs()[0]));
	}	
	else
	{
		if (!checkNickname(c.getArgs()[0]))
			c.getSender()->send_msg(ERR_ERRONEUSNICKNAME(c.getSender()->getNickname(), c.getArgs()[0]));
		else
		{
			if (c.getSender()->getIsConnected())
				c.getSender()->send_msg(":" + c.getSender()->getNickname() + " NICK " + c.getArgs()[0]);
			c.getSender()->setNickname(c.getArgs()[0]);
		}
	}
}
