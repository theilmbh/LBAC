objects = parse.o lex.o ast.o symbol.o 
CC = gcc
CFLAGS = -g

cmplr : $(objects)
	gcc -g -o cmplr $(objects)

parse.o : cmplr.h ast.h symbol.h parse.h
lex.o : cmplr.h
ast.o : ast.h
symbol.o : symbol.h cmplr.h 
