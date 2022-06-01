NAME = ft_nm
INCLUDE = -Iinclude -Ilibftprintf/include -Ilibftprintf/libft/include
HEADER = include/nm.h

SRC_DIR = ./srcs
OBJ_DIR = ./obj
SRC_EXT = c
OBJ_EXT = o
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJECTSS = $(SRCS:.c=.o)
OBJS = $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(OBJECTSS))

LIBS = libftprintf.a

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

$(NAME): directories $(LIBS) $(OBJS) $(HEADER)
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJS) $(LIBS) -o $@
	@printf "$(PINK)Done building ft_nm $(RESET)\n"

directories:
	mkdir -p $(OBJ_DIR)

%.a: %
	$(MAKE) -C $< && cp $</$@ .

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $(CFLAGS) $(INCLUDE) $^ -o $@

clean:
	/bin/rm -f $(OBJS)
	@/bin/rm -f *.o *~ *.gch
	$(MAKE) $@ -C libftprintf

fclean: clean
	/bin/rm -f $(NAME)
	$(MAKE) $@ -C libftprintf

re: fclean all
