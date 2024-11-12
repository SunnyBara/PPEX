CC=gcc
CFLAGS= -Wextra -Werror -Wall -Wvla -std=c99
BIN=myfind
MAINDIR=src
DIRINMAIN= data lib	action error ast core
SRCS= $(foreach dir,$(DIRINMAIN),$(wildcard $(MAINDIR)/$(dir)/*.c))
OBJ= $(SRCS:.c=.o)
DIRTESTS= test_myfind_core
BINTEST=testsuite.sh

all: $(BIN)

$(BIN): $(OBJ)
	gcc -o $(BIN) $(OBJ)

sanitize: $(OBJ)
	gcc -o $(BIN) $(OBJ) -fsanitize=address


test:
	./tests/$(DIRTESTS)/$(BINTEST)

clean:
	rm -rf $(BIN)
	rm -rf $(OBJ)
