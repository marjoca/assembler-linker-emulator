assembler: parser.tab.o pomocni.o lex.yy.o ulazUBazen.o simbol.o ulazURelokTabelu.o sekcija.o asembler.o main.o
	g++ -g parser.tab.o pomocni.o lex.yy.o ulazUBazen.o simbol.o ulazURelokTabelu.o sekcija.o asembler.o main.o -o assembler

linker: src/linker.cpp
	g++ -o linker src/linker.cpp

emulator: src/emulator.cpp
	g++ -o emulator src/emulator.cpp

main.o: src/main.cpp
	g++ -c src/main.cpp

asembler.o: src/asembler.cpp parser.tab.h
	g++ -c src/asembler.cpp

sekcija.o: src/sekcija.cpp
	g++ -c src/sekcija.cpp

simbol.o: src/simbol.cpp
	g++ -c src/simbol.cpp

ulazURelokTabelu.o: src/ulazURelokTabelu.cpp
	g++ -c src/ulazURelokTabelu.cpp

ulazUBazen.o: src/ulazUBazen.cpp
	g++ -c src/ulazUBazen.cpp

pomocni.o: src/pomocni.c
	g++ -c src/pomocni.c

lex.yy.o: lex.yy.c
	g++ -c lex.yy.c

lex.yy.c: inc/pomocni.h parser.tab.h misc/lexer.l
	flex misc/lexer.l

parser.tab.o: parser.tab.c
	g++ -c parser.tab.c

parser.tab.c: misc/parser.y
	bison -d -t misc/parser.y

clean:
	rm -rf *o parser.tab.c parser.tab.h lex.yy.c tests/*o assembler emulator linker tests/program.hex
