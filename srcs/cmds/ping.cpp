#include "irc.hpp"

void	cmdPing(Command &c, Server &s) {

	(void)s;
	if (!c.getSender()->getIsConnected())
	{
		return ;
	}
	std::string	str = "";
	if (c.getArgs().size() > 0)
		str += c.getArgs()[0] + " :" + c.getArgs()[0];
	c.getSender()->send_msg("PONG " + str);
}
