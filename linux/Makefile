NAME = ft_nm
INCLUDE = -Iinclude
HEADER = include/nm.h

SRC_DIR = ./srcs
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:.c=.o)

# COLORS
PINK = \x1b[35;01m
BLUE = \x1b[34;01m
YELLOW = \x1b[33;01m
GREEN = \x1b[32;01m
RED = \x1b[31;01m
WHITE = \x1b[31;37m
RESET = \x1b[0m

CFLAGS = -Wall -Werror -Wextra
ifdef DEBUG
 CFLAGS += -g3 -fsanitize=address
endif
SHELL := /bin/bash
export SHELL
export DEBUG

all: $(NAME)

$(NAME): $(LIBS) $(OBJS) $(HEADER)
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJS) $(LIBS) -o $@
	@printf "$(PINK)Done building ft_nm $(RESET)\n"

%.o: %.c
	$(CC) -c $(CFLAGS) $(INCLUDE) $^ -o $@

clean:
	/bin/rm -f $(OBJS)
	@/bin/rm -f *.o *~ *.gch

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all
