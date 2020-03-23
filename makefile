CPPFLAGS += -std=c++11 -W -Wall -g -Wno-unused-parameter
CPPFLAGS += -I include
LFLAGS = -d

all : bin/print_canonical bin/toPython #bin/eval_expr

src/c_parser.tab.cpp src/c_parser.tab.hpp : src/c_parser.y
	bison -v -d src/c_parser.y -o src/c_parser.tab.cpp

src/c_lexer.yy.cpp : src/c_lexer.flex src/c_parser.tab.hpp
	flex -o src/c_lexer.yy.cpp  src/c_lexer.flex

bin/print_canonical : src/print_canonical.o src/c_parser.tab.o src/c_lexer.yy.o src/c_parser.tab.o
	mkdir -p bin
	g++ $(CPPFLAGS) -o bin/print_canonical $^

bin/toPython : src/toPython.o src/c_parser.tab.o src/c_lexer.yy.o src/c_parser.tab.o
	mkdir -p bin
	g++ $(CPPFLAGS) -o bin/toPython $^
	
#bin/eval_expr : src/eval_expr.o src/maths_parser.tab.o src/maths_lexer.yy.o src/maths_parser.tab.o
	#mkdir -p bin
	#g++ $(CPPFLAGS) -o bin/eval_expr $^


clean :
	rm src/*.o
	rm bin/*
	rm src/*.tab.cpp
	rm src/*.yy.cpp
