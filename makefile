CPPFLAGS += -std=c++11 -W -Wall -g -Wno-unused-parameter
CPPFLAGS += -I include
LFLAGS = -d

all : bin/c_compiler bin/print_canonical bin/toPython bin/toMips

src/c_parser.tab.cpp src/c_parser.tab.hpp : src/c_parser.y
	bison -v -d src/c_parser.y -o src/c_parser.tab.cpp

src/c_lexer.yy.cpp : src/c_lexer.flex src/c_parser.tab.hpp
	flex -o src/c_lexer.yy.cpp  src/c_lexer.flex

bin/c_compiler : src/c_compiler.o src/c_parser.tab.o src/c_lexer.yy.o src/c_parser.tab.o
		mkdir -p bin
		g++ $(CPPFLAGS) -o bin/c_compiler $^

bin/print_canonical : src/print_canonical.o src/c_parser.tab.o src/c_lexer.yy.o src/c_parser.tab.o
	mkdir -p bin
	g++ $(CPPFLAGS) -o bin/print_canonical $^

bin/toPython : src/toPython.o src/c_parser.tab.o src/c_lexer.yy.o src/c_parser.tab.o
	mkdir -p bin
	g++ $(CPPFLAGS) -o bin/toPython $^

bin/toMips : src/toMips.o src/c_parser.tab.o src/c_lexer.yy.o src/c_parser.tab.o
	mkdir -p bin
	g++ $(CPPFLAGS) -o bin/toMips $^

	


clean :
	rm src/*.o
	rm bin/*
	rm src/*.tab.cpp
	rm src/*.yy.cpp
