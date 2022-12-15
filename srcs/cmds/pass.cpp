#include "irc.hpp"

void	cmdPass(Command &c, Server &s) {
	if (c.getArgs().size() != 1)
		c.getSender()->send_msg(ERR_NEEDMOREPARAMS(c, c.getSender()->getNickname()));
	else if (c.getSender()->getPsStatus() == true)
		c.getSender()->send_msg(ERR_ALREADYREGISTERED(c.getSender()->getNickname()));
	else if (c.getArgs()[0].compare(s.getPassword()))
	{
		std::cout << " > Bad Password" << std::endl;
		c.getSender()->setIsAlive(false);
	}	
	else
	{
		std::cout << " > Correct password entered" << std::endl;
		c.getSender()->setPsStatus(true);
	}
}
