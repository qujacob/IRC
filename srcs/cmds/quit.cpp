#include "irc.hpp"

void	cmdQuit(Command &c, Server &s) {
	if (c.getArgs().size() > 0)
	{
		std::string	msg = "";
		for (size_t i = 0; i < c.getArgs().size(); i++)
			msg += c.getArgs()[i] + " ";
		if (msg[0] == ':')
			msg.erase(0, 1);
	}

	for (size_t i = 0; i < s.getVctPfd().size(); i++)
	{
		if (s.getVctPfd()[i].fd == c.getSender()->getFd())
		{
			c.getSender()->setIsAlive(false);
			break;
		}
	}
}
