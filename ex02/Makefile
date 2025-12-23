NAME =   PmergeMe

COMPILER = c++

FLAGS = -std=c++98 -Wall -Wextra -Werror

HEADERS_DIRECTORY = ./header

SOURCES = PmergeMe.cpp main.cpp

OBJECTS_DIRECTORY = objects

OBJECTS = $(SOURCES:%.cpp=$(OBJECTS_DIRECTORY)/%.o)

all : $(NAME)

$(OBJECTS_DIRECTORY)/%.o : %.cpp
	mkdir -p $(@D)
	$(COMPILER) $(FLAGS) -c $< -o $@ -I $(HEADERS_DIRECTORY)

$(NAME) : $(OBJECTS)
	$(COMPILER) $(FLAGS) $(OBJECTS) -o $@ -I $(HEADERS_DIRECTORY)

clean :
	rm -rf $(OBJECTS_DIRECTORY)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY :
	all clean fclean re