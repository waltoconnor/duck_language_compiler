#
# Makefile for building scanner.  You may prefer to use cmake, especially
# if you use CLion.  I don't know cmake well enough to give you a good
# portable CMakelists.txt
#
# Local tool paths and commands  (Varies by installation)
CCOPTS = -L /usr/local/lib
# REFLEX_LIB = /usr/local/lib/libreflex.a
REFLEX_INCLUDE = /usr/local/include/reflex
REFLEX = reflex
PRODUCT = ../bin/lexer
CC = g++ -std=c++11

default: lexer

%.o: %.cpp lex.yy.h
	$(CC) -c -I $(REFLEX_INCLUDE) $^

lex.yy.cpp lex.yy.h:	quack.lxx  quack.tab.hxx
	$(REFLEX) --bison-cc --bison-locations --header-file quack.lxx

lexer:  lex_driver.cpp  lex.yy.cpp ASTNode.cpp  Messages.cpp
	$(CC) $^ -o ${PRODUCT} -I $(REFLEX_INCLUDE) $(CCOPTS) -lreflex

# There should be more dependencies for the header files, but I'm lazy.

clean:
	rm -f *.o
	rm -f lex.yy.cpp lex.yy.h
	rm -f ${PRODUCT}

