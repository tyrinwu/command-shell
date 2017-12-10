CFLAGS= -Wall -Werror -std=gnu++98 -pedantic -ggdb3

myShell: myShell.cpp myShell.h
	g++ $(CFLAGS) -o myShell myShell.cpp

clean:
	rm -f test *~
