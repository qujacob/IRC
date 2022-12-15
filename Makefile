#	COLORS	#
_NC=`tput sgr0`
_RED=\033[0;31m
_GREEN=\033[0;32m
_YELLOW=\033[0;33m
_BLUE=\033[0;34m
_PURPLE=\033[0;95m
_CYAN=\033[0;36m
_WHITE=\033[0;37m


SRCS	 	=	main.cpp \
				Server.cpp \
				Client.cpp \
				Channel.cpp \
				Command.cpp \
				replies.cpp \
				cmds/pass.cpp \
				cmds/nick.cpp \
				cmds/user.cpp \
				cmds/oper.cpp \
				cmds/mode.cpp \
				cmds/ping.cpp \
				cmds/privmsg.cpp \
				cmds/join.cpp \
				cmds/invite.cpp \
				cmds/part.cpp \
				cmds/kick.cpp \
				cmds/quit.cpp \
				cmds/kill.cpp \
				cmds/die.cpp \
				cmds/who.cpp \



INCLUDES	=	-I ./include/

CXX 			= 	c++

CXXFLAGS		= 	-Wall -Wextra -Werror -std=c++98


RM			= 	rm -rf

NAME_FT		= 	ircserv

all: 	 ${NAME_FT} 

DIR_SRCS	= ./srcs

DIR_OBJ_FT		:= obj

OBJS_FT		:= \
				$(addprefix ${DIR_OBJ_FT}/, ${SRCS:.cpp=.o})


				
$(DIR_OBJ_FT)/%.o  :	$(DIR_SRCS)/%.cpp
						@mkdir -p $(dir $@)
						@${CXX} ${CXXFLAGS} ${INCLUDES} -o $@ -c $<
						@echo "$(_YELLOW)$@ done!$(_NC)"


$(NAME_FT): 	$(OBJS_FT)
				${CXX} $(CXXFLAGS) ${INCLUDES} $(OBJS_FT) -o $(NAME_FT)
				@echo "$(_YELLOW)IRC is ready !$(_NC)"
				


clean:
				${RM} ${DIR_OBJ_FT}

fclean: 		clean
				${RM} ${NAME_FT}

re: 			fclean all

.PHONY:			all clean fclean re
