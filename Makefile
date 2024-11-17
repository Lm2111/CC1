CC=gcc
CFLAGS=-I./include -g -Werror -Wextra

DEPS = 	btree.h

OBJ = 	src/main.o \
		src/btree.o \
		src/database.o \
		src/results.o \
		src/process_input.o \
		src/sql_cell.o


src/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

database: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) 

test: 
	make clean
	make

clean:
	rm -rf database src/*.o meta.txt dbase.bin

PHONY: clean test
