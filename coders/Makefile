NAME = Codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

SRC = main.c \
	  parsing.c \
	  init.c \
	  threads.c \
	  time.c \
	  logs.c \
	  state.c \
	  monitor.c \
	  coder_actions.c \
	  coder_data.c \
	  cleanup.c \
	  dongles.c

OBJ = $(SRC:.c=.o)

#colors
GREEN = \033[0;32m
CYAN = \033[0;36m
YELLOW = \033[0;33m
RED = \033[0;31m
RESET = \033[0m

all: $(NAME)

$(NAME): $(OBJ)
	@printf "$(CYAN)\nLinking $(NAME) ...$(RESET)\n"
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@printf "\n$(GREEN)🗸$(NAME) compiled successfully$(RESET) ❤️ \n\n"

%.o: %.c codexion.h
	@printf "$(YELLOW)Compiling $<...$(RESET)\n"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJ)
	@printf "$(YELLOW)🗸 Object files removed$(RESET)\n"

fclean: clean
	@rm -f $(NAME)
	@printf "$(RED)🗸 $(NAME) removed$(RESET)\n"

re: fclean all
	@printf "$(CYAN)Rebuilding $(NAME)...$(RESET)\n"
	@$(MAKE) fclean
	@$(MAKE) all

.PHONY: all clean fclean re
