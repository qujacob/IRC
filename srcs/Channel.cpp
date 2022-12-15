#include "../include/irc.hpp"

/*
	- Channel : first character being a valid channel type prefix character.
		channel name is that it may not contain any spaces (' ', 0x20), 
		a control G / BELL ('^G', 0x07), 
		or a comma (',', 0x2C) (which is used as a list item separator by the protocol).
	-
*/

Channel::Channel(){}

Channel::Channel(std::string name) : _name(name), _invite_mode(false) {}

Channel::Channel(Channel const &src){*this = src;}

Channel &Channel::operator=(Channel const &cpy)
{
	this->_name = cpy._name;
	this->_chan_key = cpy._chan_key;
	return (*this);
}

Channel::~Channel(){}

std::string Channel::getChannelName(void) const
{
	return(this->_name);
}

std::string Channel::getTopic() const
{
	return(this->_topic);
}

bool        Channel::isTopicSet() const
{
	return(this->_set_topic);
}

bool		Channel::isChannelJoined() const
{
	return (this->_channel_joined);
}

std::vector<Client*>	&Channel::getClients()
{
	return this->_clients;
}

Client	*Channel::getClientByName(std::string name)
{
	std::vector<Client*>::iterator	it = _clients.begin();
	while (it != _clients.end())
	{
		if ((*it)->getNickname() == name)
			return (*it);
		it++;
	}
	return (NULL);
}

std::vector<Client*>    &Channel::getInvited()
{
	return this->_invited;
}

Client	*Channel::getInvitedByName(std::string name)
{
	std::vector<Client*>::iterator	it = _invited.begin();
	while (it != _invited.end())
	{
		if ((*it)->getNickname() == name)
			return (*it);
		it++;
	}
	return (NULL);
}

Client					*Channel::getOperator() const
{
	return this->_operator;
}

bool					Channel::getInviteMode(void) const
{
	return (this->_invite_mode);
}

void					Channel::setOperator(Client *client)
{
	this->_operator = client;
}

void					Channel::setInviteMode(bool invite)
{
	this->_invite_mode = invite;
}

void					Channel::send_msg(std::string msg, Client &cl)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i] != &cl)
			_clients[i]->send_msg(msg);
	}
}

void					Channel::eraseClient(Client *client)
{
	std::vector<Client*>::iterator	it = _clients.begin();
	while (it != _clients.end())
	{
		if ((*it)->getNickname() == client->getNickname())
		{
			_clients.erase(it);
			setOperator(_clients.front());
			break;
		}
		it++;
	}
}

void					Channel::eraseInvited(Client *client)
{
	std::vector<Client*>::iterator	it = _invited.begin();
	while (it != _invited.end())
	{
		if ((*it)->getNickname() == client->getNickname())
		{
			_invited.erase(it);
			break;
		}
		it++;
	}
}

void					Channel::addClient(Client &client)
{
	_clients.push_back(&client);
}
