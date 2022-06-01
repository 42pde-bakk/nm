# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: pde-bakk <marvin@codam.nl>                   +#+                      #
#                                                    +#+                       #
#    Created: 2019/12/02 17:36:51 by pde-bakk      #+#    #+#                  #
#    Updated: 2021/04/05 14:03:35 by pde-bakk      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME = libftprintf.a
INCLUDE = -I ./include_internal -I ./libft/include

SRC_DIR = src
BUILD_DIR = obj
SRC_EXT = c
OBJ_EXT = o

SOURCES := $(shell find $(SRC_DIR) -type f -name "*.$(SRC_EXT)")
OBJS    := $(SOURCES:.$(SRC_EXT)=.$(OBJ_EXT))
OBJECTS := $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(OBJS))

LIBS = libft.a

FLAGS = -Wall -Werror -Wextra
ifdef DEBUG
  FLAGS += -g -fsanitize=address
else
  FLAGS += -Ofast
endif

# COLORS
SHELL := /bin/bash
PINK = \x1b[35;01m
BLUE = \x1b[34;01m
YELLOW = \x1b[33;01m
GREEN = \x1b[32;01m
RED = \x1b[31;01m
WHITE = \x1b[31;37m
RESET = \x1b[0m

all: $(NAME)

$(NAME): directories $(OBJECTS) $(LIBS)
	@printf "$(YELLOW)Linking the library\n"
	cp libft/libft.a $(NAME)
	ar -rcs $(NAME) $(OBJECTS)
	@printf "$(GREEN)Done!$(RESET)\n"

directories:
	@mkdir -p $(BUILD_DIR)

%.a: %
	$(MAKE) -C $<

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@$(CC) -c $(FLAGS) $(INCLUDE) $^ -o $@

clean:
	@/bin/rm -f *.o *~ *.gch $(OBJECTS)
	$(MAKE) $@ -C libft

fclean: clean
	/bin/rm -f $(NAME)
	$(MAKE) $@ -C libft

re: fclean all

bonus: re
	@printf "$(PINK)Linking bonus files$(RESET)\n"
