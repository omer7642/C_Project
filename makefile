#!/bin/bash
assembly: codingFunctions.o passes.o symbol.o input.o assembly.o output.o
	gcc -g -Wall codingFunctions.o passes.o symbol.o input.o assembly.o output.o -o assembly

codingFunctions.o: codingFunctions.c codingFunctions.h 
	gcc -c -Wall codingFunctions.c -o codingFunctions.o

passes.o: passes.c passes.h
	gcc -c -Wall passes.c -o passes.o

symbol.o: symbol.c symbol.h
	gcc -c -Wall symbol.c -o symbol.o
input.o: input.c input.h
	gcc -c -Wall input.c -o input.o

assembly.o: assembly.c assembly.h
	gcc -c -Wall assembly.c -o assembly.o

output.o: output.c output.h 
	gcc -c -Wall output.c -o output.o