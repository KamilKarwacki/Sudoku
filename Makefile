SRC=./src/

WARN=-Wpedantic -Wall -Wextra
OPT=-O3

Release: main.o sudoku.o
	g++ -fno-rtti main.o sudoku.o -o ./bin/Sudoku

Debug: main_d.o sudoku_d.o
	g++ -fno-rtti main.o sudoku.o ./bin/Sudoku_d

main.o: $(SRC)main.cpp
	g++ ${WARN} ${OPT} -c $(SRC)main.cpp

sudoku.o: $(SRC)sudoku.cpp
	g++ ${WARN} ${OPT} -c $(SRC)sudoku.cpp



clean: 
	rm *.o ./bin/Sudoku
