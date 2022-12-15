#pragma once

#include "irc.hpp"

/*
	struct sockaddr_in 
	{
        short int          sin_family;  // Address family, AF_INET
        unsigned short int sin_port;    // Port number
        struct in_addr     sin_addr;    // Internet address
        unsigned char      sin_zero[8]; // Same size as struct sockaddr
    };

	struct sockaddr 
	{
        ushort  sa_family;
        char    sa_data[14];
	};
*/

class Channel;

class Server
{
	private:
		/*		[VARIABLES]	*/
		
		struct  sockaddr_in				_address;
		struct	addrinfo				_server_info;
		std::string						_port;
		std::string						_password;
		std::string						_server_name;
		std::string						_hostname;
		std::string						_date;
		std::string						_version;
		// std::string						_buffer;

		int 							_addrlen;
		int								_socket_fd;
		int								_new_socket;
		int								_nb_connection;
		size_t							_max_channels;

		struct pollfd					_pollfd;
		std::vector<pollfd>				_vct_pfd;
		std::map<int, Client*>			_mp_client;

		// std::vector<Channel *> _channels; // maybe a map of channels ?
		std::map<std::string, Channel*>	_channelsList;


		/*		[FONCTIONS] */
		
		void						new_client(int i);
		void						_loop();
		void						_msg_request(int i);
		void						_delete_user(int target, int i);
		void						_set_date();
		int							_setup_address();
		std::vector<std::string>	_parseCmds(Client *c);
		void _list_mp();

	public:
		Server();
		Server(char *port, char *psword);
		~Server();
		Server &operator=(Server const &rhs);
		void				starter();

		std::string						getPort(void) const;
		std::string						getPassword(void) const;
		std::string						getServerName(void) const;
		std::string						getHostName(void) const;
		std::string						getDate(void) const;
		std::string						getVersion(void) const;
		size_t							getMaxChannels(void) const;
		std::vector<pollfd>				getVctPfd(void) const;
		std::map<int, Client*>			getMpClient(void) const;
		Client							*getClientsByNickname(std::string name);
		std::map<std::string, Channel*>	&getChannels(void);
		Channel							*getChannelByName(std::string name);
		
		bool 							channelAlreadyCreated(std::string channelName);
		void							newChannel(std::string name);
};
