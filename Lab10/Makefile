#compiler and valgrind arguments
PROG = intTest
PROG1 = intTest_mem

#repeat_num: dataTimer repititions repeat_num2: calcTimer repititions
CC = gcc
CFLAGS = -g -std=c99 -O3 -Wall -pedantic
EN_TIME = -DEN_TIME
VALGRIND = valgrind --tool=memcheck --leak-check=yes --track-origins=yes

#source files
SOURCE = rootfinding.c intTest.c

#output files
MEM = mem.txt
OUT = output.txt

.SILENT:
.PHONY: all clean help mem test

$(PROG):
	$(CC) $(CFLAGS) $(EN_TIME) $(SOURCE) -o $(PROG)

$(PROG1):
	$(CC) $(CFLAGS) $(SOURCE) -o $(PROG1)

all: $(PROG) $(PROG1)
	@echo "compiling your binaries"

test:$(PROG) $(PROG1)
	@echo "running your test"
	./$(PROG) $(PARAMS) >> $(OUT) 2>&1

mem:$(PROG1)
	@echo "running memory test(timing disabled)"
	$(VALGRIND) ./$(PROG1) >> $(MEM) 2>&1

help:
	@echo "make options: all, clean help, mem, test"

clean:
	@echo "cleaning the current directory"
	rm -f $(OUT) $(MEM)
	rm -f $(PROG) $(PROG1)
