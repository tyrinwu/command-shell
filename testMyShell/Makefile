CFLAGS= -Wall -Werror -std=gnu++98 -pedantic -ggdb3

runTesting: test.cpp myShell.h
	g++ $(CFLAGS) -o runTesting test.cpp myShell.cpp

clean:
	rm -f test *~
