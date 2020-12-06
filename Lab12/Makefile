#compiler and valgrind arguments
PROG = hw12

#repeat_num: dataTimer repititions repeat_num2: calcTimer repititions
CC = gcc
CFLAGS = -g -std=c99 -Wall -pedantic hw12.c -lgsl -lgslcblas -o

#helper programs
GENRAND = genRand
MEMTST = memtst

#helper params
NUMBERS = 0 1 2 3 4 5 6 7 8 9 10 11 12

#input file
GE = ge
RAND = rand.txt

#output files
MEM = mem.txt
OUT = output.txt

.SILENT:
.PHONY: all clean help mem test

$(PROG):
	$(CC) $(CFLAGS) $(PROG)
	./$(GENRAND) > $(RAND)

all: $(PROG)
	@echo "compiling your binaries"

test:$(PROG)
	@echo "running your test"
	$(foreach var, $(NUMBERS), ./$(PROG) -i $(GE)$(var).txt >> $(OUT) 2>&1;)

mem:
	@echo "running memory test(timing disabled)"
	./$(MEMTST) ./$(PROG) -i rand.txt -d >> $(MEM) 2>&1

help:
	@echo "make options: all, clean help, mem, test"

clean:
	@echo "cleaning the current directory"
	rm -f $(OUT) $(MEM)
	rm -f $(PROG) $(RAND)
