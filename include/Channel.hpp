#pragma once

#include "irc.hpp"

class Command;

class Channel
{
	public:
			Channel();
			Channel(std::string name);
			Channel(Channel const &);
			Channel &operator=(Channel const &);
			~Channel();

			std::string					getChannelName() const;
			std::string 				getTopic() const;
			bool 						isTopicSet() const;
			bool 						isChannelJoined() const;
			std::vector<Client*> 		&getClients();
			Client						*getClientByName(std::string name);
			std::vector<Client*> 		&getInvited();
			Client						*getInvitedByName(std::string name);
			Client						*getOperator() const;
			bool						getInviteMode(void) const;

			void						setOperator(Client *client);
			void						setInviteMode(bool invite);

			void						send_msg(std::string msg, Client &cl);
			void						eraseClient(Client *client);
			void						addClient(Client &client);
			void						eraseInvited(Client *client);

	private:
			std::string 			_name;
			std::string				_chan_key;
			std::string				_topic;
			bool					_channel_joined;
			bool					_set_topic;
			bool 					_invite_mode;
			std::vector<Client*>	_clients;
			std::vector<Client*>	_invited;
			Client					*_operator;
};
