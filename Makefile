CC = clang++
SRC = src/
CPPFLAGS = -Wall -Wextra -Werror -pedantic -std=c++14
DFLAGS = -Wall -Wextra -pedantic -std=c++14 -g
OBJDIR = .obj/
INCLUDES = -I includes/
FILES = main VM Factory Lexer
CPPFILES = $(addsuffix .cpp, $(FILES))
OFILES = $(addprefix $(OBJDIR), $(addsuffix .o, $(FILES)))
NAME = avm
DNAME = d_$(NAME)

CL_CYAN = \033[0;36m
CL_GREEN = \033[0;32m
CL_RED = \033[0;31m
CL_WHITE = \033[0m

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJDIR) $(OFILES)
	@$(CC) $(CPPFLAGS) $(INCLUDES) $(OFILES) -o $(NAME)
	@echo "  $(CL_WHITE)+ $(NAME) : Compiled $(CL_GREEN)$@$(CL_WHITE)"

clean:
	@echo " $(shell \
		if [ -d $(OBJDIR) ]; \
		then \
			echo " $(CL_WHITE)- $(NAME) : Removing $(CL_RED)$(OBJDIR)$(CL_WHITE) with$(CL_RED)"; \
			ls $(OBJDIR) | wc -w | tr -d '[:blank:]'; echo "$(CL_WHITE)object files"; \
			rm -Rf $(OBJDIR); \
		else \
			echo " $(CL_WHITE)# $(NAME) : Nothing to clean"; \
		fi) "

fclean: clean
	@echo " $(shell \
		if [ -f $(NAME) ]; \
		then \
			echo " $(CL_WHITE)- $(NAME) : Removing  $(CL_RED)$(NAME)$(CL_WHITE)"; \
			rm -f $(NAME); \
		else \
			echo " $(CL_WHITE)# $(NAME) : Nothing to fclean"; \
		fi) "

re: fclean all

$(addprefix $(OBJDIR), %.o): $(addprefix $(SRC), %.cpp)
	@$(CC) $(INCLUDES) $(CPPFLAGS) -o $@ -c $<

$(OBJDIR):
	@echo " $(shell \
	if [ ! -d $(OBJDIR) ]; \
	then \
		mkdir $(OBJDIR); \
		echo " + $(NAME) : Created  $(CL_GREEN)$(OBJDIR)$(CL_WHITE)$(CL_WHITE)"; \
fi) "

j:
	$(CC) $(DFLAGS) $(INCLUDES) $(addprefix $(SRC), $(CPPFILES)) -o $(DNAME)

k: fclean
	rm -f $(DNAME)
	rm -rf *.dSYM
