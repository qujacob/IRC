#include "../include/irc.hpp"
#include <unistd.h>

bool _status = true;
/*
	Setup the server
	- https://fr.acervolima.com/programmation-de-sockets-en-c-c/
	- [FR] http://abcdrfc.free.fr/rfc-vf/rfc1459.html
*/
void _sig(int signum)
{
	if (signum == SIGINT)
		_status = false;
}

int main(int ac, char **av)
{
	if (ac != 3) {
		std::cerr << "Usage : ./ircserv <port> <password>" << std::endl;
		return (1);
	}
	if (!av[2][0]) {
		std::cerr << "Invalid password" << std::endl;
		return (1);
	}
	for (int i = 0; av[2][i]; i++) {
		if (std::isspace(av[2][i])) {
			std::cerr << "Invalid password" << std::endl;
			return (1);
		}
	}

	signal(SIGINT, _sig);
	signal(SIGQUIT, _sig);
	Server	_server(av[1], av[2]);
	_server.starter();
	return (0);
}
