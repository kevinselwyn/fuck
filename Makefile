NAME    := fuck
NAME_PG := care
BIN_DIR := /usr/local/bin

all: build

build: main.c
	gcc -o $(NAME) $<

test: build
	./$(NAME) -c
	./$(NAME)
	./$(NAME)
	./$(NAME)
	./$(NAME) -l
	./$(NAME) -r
	./$(NAME) -c

install: $(NAME)
	install -m 0755 $(NAME) $(BIN_DIR)

uninstall:
	rm -f $(BIN_DIR)/$(NAME)

pg: main.c
	gcc -o $(NAME_PG) $< -DEXPLETIVE=\"care\"

pg_install: $(NAME_PG)
	install -m 0755 $(NAME_PG) $(BIN_DIR)

pg_uninstall:
	rm -f $(BIN_DIR)/$(NAME_PG)

clean:
	rm -f $(NAME) $(NAME_PG)