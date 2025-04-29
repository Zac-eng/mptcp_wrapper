NAME=mptcp_wrapper
CXX=g++
CXXFLAGS=-Wall -Wextra -Werror -std=c++11 -Isrc
SRC_DIR=./src
OBJ_DIR=./object
RM=rm -rf

SRCS=$(shell find $(SRCDIR) -type f -name '*.cc')
OBJS=$(subst $(SRC_DIR),$(OBJ_DIR),$(SRCS:.cc=.o))

build: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	echo $(SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $@

run: build
	$(NAME)

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean build
