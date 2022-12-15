#include "../include/irc.hpp"

/*
	RFC 2812 -> https://www.rfc-editor.org/rfc/rfc2812#section-3.1
	irssi --connect=localhost --port=8080	
*/
int	Server::_setup_address()
{
	struct addrinfo hints, *ai, *p;
	int opt = 1;
	
	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	
	if ((getaddrinfo(_hostname.c_str(), _port.c_str(), &hints, &ai)) != 0) {
		std::cerr << "getaddrinfo failed";
		std::exit(1);
	}
	for (p = ai; p != NULL; p = p->ai_next)
	{

		if ((_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			std::cerr << _RED "Socket failed !\n" _NC;
			exit(1);
		}
		if (_socket_fd < 0)
			continue;
		if (setsockopt(_socket_fd, SOL_SOCKET , SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
			std::cerr << _RED "setsocket error\n" _NC;
			exit(1);
		}
		/*	Link the socket address and the port number*/
		if (bind(_socket_fd, p->ai_addr, p->ai_addrlen) < 0) {
			close(_socket_fd);
			continue;
		}
		break;
	}
	freeaddrinfo(ai);
	if (p == NULL)
		return (-1);
	return (0);
}

void	Server::_set_date()
{
	time_t rawtime;
	struct tm * timeinfo;
	char buffer [80];

	time (&rawtime);
	timeinfo = localtime (&rawtime);
	strftime(buffer, 80,"%A %d %B %G", timeinfo);
	this->_date = buffer;
}

Server::Server(char *port, char *psword) : _port(port), _password(psword), _server_name("Los tankos"),\
	 _hostname("0.0.0.0"), _version("1.0"), _nb_connection(0), _max_channels(6)
{
	std::cout << _YELLOW << "~~~ Initialization of the server ~~~"<< _NC << std::endl;
	if (_setup_address() == -1)
	{
		std::cerr << "Address error\n";
		std::exit(1);
	}
	_set_date();
	// convert the port (char * -> int)
    int i = 0;
 	std::istringstream(port) >> i;
	
	// set the struct
	std::memset(&_address, 0, sizeof _address);
	_address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons(i);
    _addrlen = sizeof(_address);

	_vct_pfd.push_back(pollfd());
	_vct_pfd[0].fd = _socket_fd;
	_vct_pfd[0].events = POLLIN;
	_vct_pfd[0].revents = 0;
}

Server::~Server()
{
	std::cout << _YELLOW << "~~~ Server shutdown bye bye ~~~\n"<< _NC;
	close(_new_socket);
	shutdown(_socket_fd, SHUT_RDWR);
}

std::vector<std::string>	Server::_parseCmds(Client *c) {
	std::string					val;
	std::stringstream			data(c->getBuffer());
    const char					sep = '\n';
    std::vector<std::string>	cmd;
	while (std::getline(data, val, sep)) {
		if (val[val.length() - 1] == '\r')
			val.erase(val.length() - 1);
		cmd.push_back(val);
	}
	c->setBuffer("");
	return cmd;
}

void	Server::_msg_request(int i)
{
	int ret = 0;
	int target = _vct_pfd[i].fd;

	char test[MAX_MSG] = {0};

	std::memset(test, 0, MAX_MSG);
	ret = recv(target, test, MAX_MSG, 0);
	if (ret <= 0)
	{
		if (ret == 0) // remove client
			_delete_user(target, i);
		else // error recv
			perror("recv");
		return ;
	}
	_mp_client[target]->setBuffer(_mp_client[target]->getBuffer() + test);

	if (_mp_client[target]->getBuffer().find('\n') == std::string::npos)
		return ;
	std::cout << " < " << _mp_client[target]->getBuffer();
	
	std::vector<Command>		cmds_v;
	std::vector<std::string>	parsed_v = _parseCmds(_mp_client[target]);

	for (size_t j = 0; j < parsed_v.size(); j++)
	{
		Command	cmd(_mp_client[target], parsed_v[j]);
		cmds_v.push_back(cmd);
	}
	for (size_t j = 0; j < cmds_v.size(); j++)
	{
		std::map<std::string, Command::cmdFunc>::const_iterator	it = cmds_v[j].getCodex().find(cmds_v[j].getCmd());
		std::map<std::string, Command::cmdFunc>::const_iterator	it_end = cmds_v[j].getCodex().end();
		if (it != it_end)
			cmds_v[j].getCodex().at(cmds_v[j].getCmd())(cmds_v[j], *this);
	}
}

void	Server::_loop()
{
	while (_status)
	{
		poll(_vct_pfd.data(), _vct_pfd.size(), -1); // -1 flag for the timeout of poll
		for (size_t i = 0; i < (size_t)_vct_pfd.size(); i++)
		{
			if (_status && _vct_pfd[i].revents & POLLIN)
			{
				if (_vct_pfd[i].fd == _socket_fd)
					new_client(i);
				else if (_status) {
					_msg_request(i);
					if (_mp_client[_vct_pfd[i].fd] && !_mp_client[_vct_pfd[i].fd]->getIsAlive()) // remove client
						_delete_user(_vct_pfd[i].fd, i);
				}
			}
		}
	}
	std::map<std::string, Channel*>::iterator	it_chann = _channelsList.begin();
	while (it_chann != _channelsList.end())
	{
		it_chann->second->getClients().clear();
		delete it_chann->second;
		it_chann++;
	}
	_channelsList.clear();

	for(size_t i = 0; i < _vct_pfd.size(); i++)
		close(_vct_pfd[i].fd);
	std::map<int, Client*>::iterator it = _mp_client.begin();
	while (it != _mp_client.end()) {
		delete it->second;
		it++;
	}
	_mp_client.clear();
	_vct_pfd.clear();
}

void	Server::_delete_user(int target, int i)
{
	std::cout << _GREEN "Bye to fd -> " << target << _NC << std::endl;
	
	std::vector<Channel*>::iterator it_chann = _mp_client[target]->getChannels().begin();
	while (it_chann != _mp_client[target]->getChannels().end())
	{
		(*it_chann)->eraseClient(_mp_client[target]);
		it_chann++;
	}
	_mp_client[target]->getChannels().clear();

	std::vector<pollfd>::iterator it = _vct_pfd.begin();
	std::advance(it, i);
	close(target);
	_vct_pfd.erase(it);
	delete _mp_client[target];
	_mp_client[target] = NULL;
	_mp_client.erase(target);
	_nb_connection--;
}
/*
	Server's begin
*/

void    Server::starter()
{
	/* Waiting list of fd */
	if (listen(_socket_fd, MAX_CLIENT) < 0)
	{
		std::cerr << _RED "Listen error\n" _NC;
		std::exit(1);
	}
	_loop();
}

void	Server::new_client(int i)
{
	struct sockaddr_in		addr;
	socklen_t			addr_len;
	(void)i;
	std::memset(&addr, 0, sizeof(addr));
	std::memset(&addr_len, 0, sizeof(addr_len));
	if ((_new_socket = accept(_socket_fd, (struct sockaddr *)&addr, &addr_len)) == -1)
	{
		std::cerr << _RED "Accept error\n" _NC;
		return ;
	}
	if (_nb_connection < MAX_CLIENT)
	{
		if ((getsockname(_new_socket, (struct sockaddr *)&addr, &addr_len)) == -1) 
		{
			std::cerr << "Error: getsockname()" << std::endl;
			close(_new_socket);
			return ;
		}
		Client *cl = new Client(_new_socket, addr);
		_vct_pfd.push_back(_pollfd);
		_vct_pfd.back().fd = _new_socket;
		_vct_pfd.back().events = POLLIN;
		_vct_pfd.back().revents = 0;
		_mp_client[_new_socket] = cl;
		_nb_connection++;
		std::cout << _GREEN "New connection from -> " << inet_ntoa(addr.sin_addr) << " and his fd -> " << _vct_pfd.back().fd << _NC << std::endl;
	}
	return ;
}

Client	*Server::getClientsByNickname(std::string name) {
	std::map<int, Client*>::iterator	it = _mp_client.begin();
	while (it != _mp_client.end())
	{
		if (it->second->getNickname() == name)
			return (it->second);
		it++;
	}
	return (NULL);
}

Channel	*Server::getChannelByName(std::string name) {
	std::map<std::string, Channel*>::iterator	it = _channelsList.begin();
	while (it != _channelsList.end())
	{
		if (it->second->getChannelName() == name)
			return (it->second);
		it++;
	}
	return (NULL);
}

bool Server::channelAlreadyCreated(std::string channelName)
{
	if (_channelsList.find(channelName) != _channelsList.end())
		return (true);
	return (false);
}

void	Server::newChannel(std::string name)
{
	_channelsList[name] = new Channel(name);
}

/*	GETTERS*/

std::string	Server::getPort(void) const
{
	return (this->_port);
}

std::string	Server::getPassword(void) const
{
	return (this->_password);
}

std::string	Server::getServerName(void) const
{
	return(this->_server_name);
}

std::string	Server::getHostName(void) const
{
	return (this->_hostname);
}

std::string	Server::getDate(void) const
{
	return (this->_date);
}

std::string	Server::getVersion(void) const
{
	return (this->_version);
}

size_t							Server::getMaxChannels(void) const
{
	return (this->_max_channels);
}

std::vector<pollfd>				Server::getVctPfd(void) const
{
	return (this->_vct_pfd);
}

std::map<int, Client*>			Server::getMpClient(void) const {
	return (this->_mp_client);
}

std::map<std::string, Channel*>	&Server::getChannels(void) {
	return (this->_channelsList);
}
