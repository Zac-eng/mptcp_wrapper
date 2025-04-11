NAME=mptcp_tunnel
CPP=g++
CPPFLAGS=-Wall -Wextra -Werror
SRCS= $(wildcard *.cpp)
OBJS= $(SRCS:.cpp=.o)
RM=rm -rf

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS)
	$(CPP) $(CPPFLAGS) $(OBJS) -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all