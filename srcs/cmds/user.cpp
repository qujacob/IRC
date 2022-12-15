#include "irc.hpp"

void	cmdUser(Command &c, Server &s) {
	Client	*usr = c.getSender();

	if (!usr->getPsStatus())
	{
		return ;
	}
	else if (c.getArgs().size() < 4)
		usr->send_msg(ERR_NEEDMOREPARAMS(c, usr->getNickname()));
	else if (c.getSender()->getIsConnected())
		usr->send_msg(ERR_ALREADYREGISTERED(usr->getNickname()));
	else
	{
		usr->setUsername(c.getArgs()[0]);
		usr->setHostname(c.getArgs()[1]);
		usr->setServername(c.getArgs()[2]);
		
		for (size_t i = 3; i < c.getArgs().size(); i++)
		{
			usr->setRealname(usr->getRealname() + c.getArgs()[i]);
			if (i != c.getArgs().size() - 1)
				usr->setRealname(usr->getRealname() + " ");
		}
		if (usr->getRealname()[0] == ':')
			usr->setRealname(usr->getRealname().erase(0, 1));
		
		usr->send_msg(RPL_WELCOME(*usr));
		usr->send_msg(RPL_YOURHOST(s, usr->getNickname()));
		usr->send_msg(RPL_CREATED(s, usr->getNickname()));
		c.getSender()->setIsConnected(true);
	}
}
