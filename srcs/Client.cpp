#include "../include/irc.hpp"

/*
    User's name = Max lenghth size -> 9
    
*/

Client::Client() {}

Client::Client(int fd, struct sockaddr_in addr)
{
	_alive = true;
	_ps_status = false;
	_connected = false;
	_isOp = false;
	_fd = fd;
	_addr = addr;
	_nickname = "*";
	_username = "";
	_realname = "";
	_max_channels = 5;
}

Client::~Client() {}

std::string				Client::getNickname(void) const {
	return (this->_nickname);
}

std::string				Client::getUsername(void) const {
	return (this->_username);
}

std::string				Client::getRealname(void) const {
	return (this->_realname);
}

std::string				Client::getHostname(void) const {
	return (this->_hostname);
}

std::string				Client::getServername(void) const {
	return (this->_servername);
}

bool					Client::getPsStatus(void) const {
	return (this->_ps_status);
}

bool					Client::getIsOp(void) const {
	return (this->_isOp);
}

int						Client::getFd(void) const {
	return (this->_fd);
}

bool					Client::getIsConnected(void) const {
	return (this->_connected);
}

bool					Client::getIsAlive(void) const {
	return (this->_alive);
}

size_t					Client::getMaxChannels(void) const {
	return (this->_max_channels);
}

std::vector<Channel*>	&Client::getChannels(void) {
	return (this->_channels);
}

Channel					*Client::getChannelByName(std::string name) {
	std::vector<Channel*>::iterator	it = _channels.begin();
	while (it != _channels.end())
	{
		if ((*it)->getChannelName() == name)
			return (*it);
		it++;
	}
	return (NULL);
}

std::string				Client::getBuffer(void) const {
	return (this->_buffer);
}

void					Client::setNickname(std::string nickname) {
	this->_nickname = nickname;
}

void					Client::setUsername(std::string username) {
	this->_username = username;
}

void					Client::setRealname(std::string realname) {
	this->_realname = realname;
}

void					Client::setHostname(std::string hostname) {
	this->_hostname = hostname;
}

void					Client::setServername(std::string servername) {
	this->_servername = servername;
}

void					Client::setPsStatus(bool status) {
	this->_ps_status = status;
}

void					Client::setIsAlive(bool alive) {
	this->_alive = alive;
}

void					Client::setOp(bool op) {
	this->_isOp = op;
}

void					Client::setIsConnected(bool connected) {
	this->_connected = connected;
}

void					Client::setBuffer(std::string buffer) {
	this->_buffer = buffer;
}

void					Client::send_msg(std::string request) const
{
	request += END;
	if((send(_fd, request.c_str(), request.length(), 0)) == -1)
		std::cerr << "Error send" << std::endl;
	std::cout << " > " << request;
}

void					Client::eraseChannel(Channel *channel)
{
	std::vector<Channel*>::iterator	it = _channels.begin();
	while (it != _channels.end())
	{
		if ((*it)->getChannelName() == channel->getChannelName())
		{
			_channels.erase(it);
			break;
		}
		it++;
	}
}

void					Client::addChannel(Channel &channel)
{
	_channels.push_back(&channel);
}
