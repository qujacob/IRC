#pragma once

#include "irc.hpp"

class Command {

public:

	typedef void (*cmdFunc)(Command &, Server &);

	Command(Client *c, std::string parse);
	~Command();

	Client									*getSender(void) const;
	std::string								getCmd(void) const;
	std::vector<std::string>				getArgs(void) const;
	const std::map<std::string, cmdFunc>	&getCodex(void) const;

private:

	Client							*_sender;
	std::string						_cmd;
	std::vector<std::string>		_args;
	std::map<std::string, cmdFunc>	_cmdCodex;

	std::vector<std::string>    _parseCmd(std::string parse);
	void						_createCodex(void);

};

/*		[COMMAND]	*/

void	cmdPass(Command &c, Server &s);
void	cmdNick(Command &c, Server &);
void	cmdUser(Command &c, Server &);
void	cmdOper(Command &c, Server &s);
void	cmdMode(Command &c, Server &s);
void	cmdPing(Command &c, Server &s);
void	cmdPrivMsg(Command &c, Server &s);
void	cmdJoin(Command &c, Server &s);
void	cmdInvite(Command &c, Server &s);
void	cmdPart(Command &c, Server &s);
void	cmdKick(Command &c, Server &s);
void	cmdQuit(Command &c, Server &s);
void	cmdKill(Command &c, Server &s);
void	cmdDie(Command &c, Server &s);
void	cmdWho(Command &c, Server &s);

/*		[BOT]		*/

void	bot_msg(Client *sender, Channel *channel);
