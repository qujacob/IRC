#pragma once

#include "irc.hpp"

class Server;
class Channel;

class Client
{
private:

	std::string				_nickname;
	std::string				_username;
	std::string				_realname;
	std::string				_hostname;
	std::string				_servername;
	bool					_alive;
	bool					_ps_status;
	bool					_connected;
	bool					_isOp;
	int						_fd;
	size_t					_max_channels;
	struct sockaddr_in		_addr;
	std::vector<Channel*>	_channels;

	std::string				_buffer;

public:
	Client();
	Client(int fd);
	Client(int fd, struct sockaddr_in addr);
	~Client();
	
	std::string				getNickname(void) const;
	std::string				getUsername(void) const;
	std::string				getRealname(void) const;
	std::string				getServername(void) const;
	std::string				getHostname(void) const;
	int						getFd(void) const;
	bool					getPsStatus(void) const;
	bool					getIsOp(void) const;
	bool					getIsConnected(void) const;
	bool					getIsAlive(void) const;
	size_t					getMaxChannels(void) const;
	std::vector<Channel*>	&getChannels(void);
	Channel					*getChannelByName(std::string name);
	std::string				getBuffer(void) const;

	void					setNickname(std::string nickname);
	void					setUsername(std::string username);
	void					setRealname(std::string realname);
	void					setHostname(std::string hostname);
	void					setServername(std::string servername);
	void					setPsStatus(bool status);
	void					setIsAlive(bool alive);
	void					setOp(bool op);
	void					setIsConnected(bool connected);
	void					setBuffer(std::string buffer);

	void					check_log(Server *serv, Client c);
	void					send_msg(std::string request) const;
	void					eraseChannel(Channel *channel);
	void					addChannel(Channel &channel);
};
