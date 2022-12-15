#include "irc.hpp"

void	cmdOper(Command &c, Server &s) {
	(void)s;
	Client	*sender = c.getSender();
	if (!c.getSender()->getIsConnected())
	{
		return ;
	}
	if (c.getArgs().size() != 2)
		sender->send_msg(ERR_NEEDMOREPARAMS(c, sender->getNickname()));
	else if (c.getArgs()[0] != sender->getNickname())
	{
		sender->send_msg(ERR_USERSDONTMATCH(sender->getNickname()));
	}
	else if (c.getArgs()[1].compare("root"))
		sender->send_msg(ERR_PASSWDMISMATCH(sender->getNickname()));
	else {
		sender->send_msg(RPL_YOUREOPER(sender->getNickname()));
		sender->setOp(true);
	}
}
