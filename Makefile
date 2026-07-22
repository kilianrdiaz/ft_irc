NAME = ircserv

SRCS_DIR = src
INC_DIR  = inc
OBJS_DIR = obj

SRCS = main.cpp \
       Server.cpp \
       Client.cpp \
       command_handler/exceptions.cpp \
       command_handler/ACommand_Handler.cpp \
       command_handler/AChannelCommandHandler.cpp \
       command_handler/Pass.cpp \
       command_handler/User.cpp \
       command_handler/Nick.cpp \
       command_handler/Cap.cpp \
       command_handler/Ping.cpp \
       command_handler/Join.cpp \
       command_handler/Part.cpp \
       command_handler/Topic.cpp \
       command_handler/Mode.cpp \
       command_handler/Kick.cpp \
       command_handler/Invite.cpp \
       command_handler/Privmsg.cpp \
       Channel.cpp

OBJS = $(SRCS:%.cpp=$(OBJS_DIR)/%.o)
DEPS = $(OBJS:.o=.d)

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -I$(INC_DIR) -MMD -MP

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re