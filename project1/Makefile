CC=gcc
CFLAGS=-I.
DEPS = parse.h y.tab.h
OBJ = y.tab.o lex.yy.o parse.o example.o
FLAGS = -g -Wall

default:all

all: example echo_server echo_client

lex.yy.c: lexer.l
	flex $^

y.tab.c: parser.y
	yacc -d $^

%.o: %.c $(DEPS)
	$(CC) $(FLAGS) -c -o $@ $< $(CFLAGS)

example: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

echo_server: y.tab.o lex.yy.o parse.o echo_server.o
	$(CC) -o $@ $^ -Wall -Werror

echo_client:
	$(CC) echo_client.c -o echo_client -Wall -Werror

clean:
	rm -f *~ *.o example lex.yy.c y.tab.c y.tab.h echo_server echo_client

test:
	./cp1_checker.py 127.0.0.1 9999 1000 10 500

test2:
	./check.py 127.0.0.1 9999 1000 10 2048 500
