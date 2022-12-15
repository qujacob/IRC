#include "irc.hpp"

Command::Command(Client *c, std::string parse) {
	_createCodex();
	_sender = c;
	if (parse.empty())
		return;
	_args = _parseCmd(parse);
	// _cmd = "NICK";
	_cmd = _args[0];
	_args.erase(_args.begin());
}

Command::~Command() {
}

std::vector<std::string>	Command::_parseCmd(std::string parse) {
	std::stringstream data(parse);
    const char sep = ' ';
    std::vector<std::string> cmd;
    std::string    val;
    while (std::getline(data, val, sep))
        cmd.push_back(val);
    return cmd;
}

void						Command::_createCodex(void) {
	_cmdCodex["PASS"] = cmdPass;
	_cmdCodex["NICK"] = cmdNick;
	_cmdCodex["USER"] = cmdUser;
	_cmdCodex["OPER"] = cmdOper;
	_cmdCodex["MODE"] = cmdMode;
	_cmdCodex["PING"] = cmdPing;
	_cmdCodex["PRIVMSG"] = cmdPrivMsg;
	_cmdCodex["JOIN"] = cmdJoin;
	_cmdCodex["INVITE"] = cmdInvite;
	_cmdCodex["PART"] = cmdPart;
	_cmdCodex["KICK"] = cmdKick;
	_cmdCodex["QUIT"] = cmdQuit;
	_cmdCodex["kill"] = cmdKill;
	_cmdCodex["die"] = cmdDie;
	_cmdCodex["WHO"] = cmdWho;
}

Client											*Command::getSender(void) const {
	return (this->_sender);
}

std::string										Command::getCmd(void) const {
	return (this->_cmd);
}

std::vector<std::string>						Command::getArgs(void) const {
	return (this->_args);
}

const std::map<std::string, Command::cmdFunc>	&Command::getCodex(void) const {
	return (this->_cmdCodex);
}
