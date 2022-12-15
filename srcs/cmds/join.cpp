#include "irc.hpp"

/*
	JOIN
	Parameters: <channel>{,<channel>} [<key>{,<key>}]
	- The server receiving the command checks whether or not the client can join the given channel, and processes the request. 
	Servers MUST process the parameters of this command as lists on incoming commands from clients, with the first <key> being 
	used for the first <channel>, the second <key> being used for the second <channel>, etc.
*/


void	createChannel(Command &c, Client &cl, Server &s)
{
	std::string			name;
	std::stringstream 	input_stringstream(c.getArgs()[0]);
	while (std::getline(input_stringstream, name, ','))
	{
		if (name.size() > 1 && name[0] == '#')
		{
			if	(!s.channelAlreadyCreated(name))
			{
				if (cl.getMaxChannels() <= cl.getChannels().size())
					cl.send_msg(ERR_TOOMANYCHANNELS(name, cl.getNickname()));
				else
				{
					s.newChannel(name);
					s.getChannels()[name]->addClient(*c.getSender());
					s.getChannels()[name]->setOperator(c.getSender());
					cl.addChannel(*s.getChannels()[name]);
					std::vector<Client*> clients = s.getChannels()[name]->getClients();
					for (size_t i = 0; i < clients.size(); i++)
							clients[i]->send_msg(":" + cl.getNickname() + "!" + cl.getUsername() + "@" + cl.getServername() + " JOIN " + name);
				}
			}
			else
			{
				Channel *channel = s.getChannelByName(name);
				if (cl.getMaxChannels() <= cl.getChannels().size())
					cl.send_msg(ERR_TOOMANYCHANNELS(name, cl.getNickname()));
				
				// check if invite only (check si le channel est en invite mode et qu'il n'est pas dans la liste des invites)
				else if (channel->getInviteMode() && !(channel->getInvitedByName(cl.getNickname())))
				{
					cl.send_msg(ERR_INVITEONLYCHAN(cl.getNickname(), channel->getChannelName()));
				}
				else
				{
					std::cout << &channel->getInvited() << std::endl;
					channel->addClient(*c.getSender());
					cl.addChannel(*channel);
					std::vector<Client*> clients = channel->getClients();
					for (size_t i = 0; i < clients.size(); i++)
							clients[i]->send_msg(":" + cl.getNickname() + "!" + cl.getUsername() + "@" + cl.getServername() + " JOIN " + name);
					Client	*invite = channel->getInvitedByName(cl.getNickname());
					if (invite)
					{
						channel->eraseInvited(channel->getInvitedByName(cl.getNickname()));
					}
				}
			}
		}
		else
		{
			cl.send_msg(ERR_NOSUCHCHANNEL(name, cl.getNickname()));
		}
	}
}

void	cmdJoin(Command &c, Server &s) 
{
	if (!c.getSender()->getIsConnected())
	{
		return ;
	}
 	if (c.getArgs().size() < 1)
	{
		c.getSender()->send_msg(ERR_NEEDMOREPARAMS(c, c.getSender()->getNickname()));
		return ;
	}
	createChannel(c, *c.getSender(), s);
}
