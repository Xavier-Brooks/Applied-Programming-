#compiler and valgrind arguments
PROG = quadTest
PROG1 = quadTest_mem

#repeat_num: dataTimer repititions repeat_num2: calcTimer repititions
CC = gcc
CFLAGS = -g -std=c99 -O2 -Wall -pedantic
EN_TIME = -DEN_TIME -DREPEAT_NUM=100
VALGRIND = valgrind --tool=memcheck --leak-check=yes --track-origins=yes

#source files
SOURCE = quadTest.c

#command line arguments
PARAMS = -a 0 -b 0 -c 1
PARAMS_1 = -a 1 -b 0
PARAMS_2 = -a 1 -b 0 -c 1

#output files
MEM = mem.txt
OUT = output.txt

.SILENT:
.PHONY: all clean help mem test

$(PROG):
	$(CC) $(CFLAGS) $(EN_TIME) $(SOURCE) -o $(PROG) -lm

$(PROG1):
	$(CC) $(CFLAGS) $(SOURCE) -o $(PROG1) -lm

all: $(PROG) $(PROG1)
	@echo "compiling your binaries"

test:$(PROG) $(PROG1)
	@echo "running you tests"
	@echo "running test 1" >> $(OUT)
	./$(PROG) $(PARAMS) >> $(OUT) 2>&1

	@echo "running test 2" >> $(OUT)
	./$(PROG) $(PARAMS_1) >> $(OUT) 2>&1

	@echo "running test3" >> $(OUT)
	./$(PROG) $(PARAMS_2) >> $(OUT) 2>&1

mem:$(PROG1)
	@echo "running memory test(timing disabled)"
	$(VALGRIND) ./$(PROG1) $(PARAMS) >> $(MEM) 2>&1

help:
	@echo "make options: all, clean help, mem, test"

clean:
	@echo "cleaning the current directory"
	rm -f $(OUT) $(MEM)
	rm -f $(PROG) $(PROG1)
