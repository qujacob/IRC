#pragma once

#include <iostream>
#include <string>
#include <cstring>

#include <vector>
#include <map>
#include <list>
#include <algorithm>

/*Server*/
#include <cstdlib>
#include <ctime>
#include <netdb.h> //gethostbyname4
#include <functional> // bind 
#include <poll.h> // poll
#include <sys/socket.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <sstream> // istringstream
#include <unistd.h>
#include <netinet/in.h> //htons
#include <arpa/inet.h>

#include <stdio.h>
#include <string.h>

#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Command.hpp"
#include "replies.hpp"

#include <signal.h>

#define _NC "\033[0;0m"
#define _RED "\033[0;31m"
#define _GREEN "\033[0;32m"
#define _YELLOW "\033[4;33m"
#define _BLUE "\033[0;34m"
#define _PURPLE "\033[0;95m"
#define _CYAN "\033[1;36m"
#define _WHITE "\033[0;37m"

/* [RFC SOURCE] Max size of an IRC chat 510 (character) + 1 ('\n') + 1 ('\0') */
#define MAX_MSG	512

/*	Max client define by ourself*/
#define MAX_CLIENT	12

/* Define end of command */
#define END "\r\n"

extern bool					_status; // sig CTRL + D
