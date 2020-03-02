##
## EPITECH PROJECT, 2019
## PSU_2019_malloc
## File description:
## Makefile
##

# Names
NAME	=	malloc.so

# Directories
INC_DIR	=	./include/
SRC_DIR	=	./src/
OBJ_DIR	=	./obj/

# Output function
ECHO	=	echo -e
ERROR_COLOR		=	sed -E "s/error/`printf "\\e[1;31m"`error`printf "\\e[0m"`/"
WARNING_COLOR	= 	sed -E "s/warning/`printf "\\e[1;33m"`warning`printf "\\e[0m"`/"
SED_BOLD		=	sed -E "s/\‘\S*\’/`printf "\\e[1m"`&`printf "\\e[0m"`/g"

# Colors
DEFAULT	=	\e[0m
BOLD	=	\e[1m
BLACK	=	\e[30m
RED		=	\e[31m
GREEN	=	\e[32m
YELLOW	=	\e[33m
BLUE	=	\e[34m
MAGENTA	=	\e[35m
CYAN	=	\e[36m
WHITE	=	\e[37m
BG_BLACK	=	\e[40m
BG_RED		=	\e[41m
BG_GREEN	=	\e[42m
BG_YELLOW	=	\e[43m
BG_BLUE	=	\e[44m
BG_MAGENTA	=	\e[45m
BG_CYAN	=	\e[46m
BG_WHITE	=	\e[47m

# Compile Strings
OK			=	$$'$(BG_GREEN)  OK  $(DEFAULT)  '
INFO			=	$$'$(BG_BLUE) INFO $(DEFAULT)  '
WARNING			=	$$'$(BG_YELLOW) WARNING $(DEFAULT)  '
ERROR			=	$$'$(BG_RED) ERROR $(DEFAULT)  '
CLEAN			=	$(INFO)$$'Cleaning...$(DEFAULT)'

# Object Compile Status
define compile_check
	@$(1) 2> $@.log; \
	RESULT=$$?; \
	if [ $$RESULT -ne 0 ]; then \
		$(ECHO) $(ERROR) $$'$(@F)'; \
		cat $@.log | sed -E "s/^/`printf "\\e[1;31m"`>`printf "\\e[0m"`  /" | $(ERROR_COLOR) | $(WARNING_COLOR) | $(SED_BOLD);	\
	elif [ -s $@.log ]; then \
		$(ECHO) $(WARNING) $$'$(@F)'; \
		cat $@.log | sed -E "s/^/`printf "\\e[1;33m"`>`printf "\\e[0m"`  /" | $(ERROR_COLOR) | $(WARNING_COLOR) | $(SED_BOLD);	\
	else \
		$(ECHO) $(OK) $$'$(@F)'; \
		cat $@.log | sed -E "s/^/`printf "\\e[1;32m"`>`printf "\\e[0m"`  /" | $(ERROR_COLOR) | $(WARNING_COLOR) | $(SED_BOLD);	\
	fi
	@$(RM) $@.log;
	@exit $$RESULT
endef

# Objects creation
$(OBJ_DIR)%.o : $(SRC_DIR)%.c
	@mkdir -p $(@D)
	$(call compile_check,$(CC) $(CFLAGS) -c -o $@ $<)

# Compilation
RM		=	rm -f
SRCS	=	$(wildcard $(SRC_DIR)*.c)
OBJS	=	$(SRCS:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)
CFLAGS	+=	-I$(INC_DIR) -fPIC -shared

all:	$(NAME)

$(NAME):	$(OBJS)
	@$(CC) $(CFLAGS) -shared -o $(NAME) $(OBJS)
	@$(call compile_check)

debug:	CFLAGS += -g -DDEBUG
debug:	fclean $(NAME)

clean:
	@$(ECHO) $(CLEAN)
	@$(RM) -r $(OBJ_DIR)

fclean:	clean
	@$(RM) $(NAME)

re:	fclean all

.PHONY:	all debug tests_run clean fclean re
