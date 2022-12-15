#include "irc.hpp"

void	cmdDie(Command &c, Server &s) {
	(void)s;
	if (!c.getSender()->getIsConnected())
	{
		return ;
	}
	if (!c.getSender()->getIsOp())
		c.getSender()->send_msg(ERR_NOPRIVILEGES(c.getSender()->getNickname()));
	else
		_status = 0;
}
