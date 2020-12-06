#compiler and valgrind arguments
PROG = hw13

#repeat_num: dataTimer repititions repeat_num2: calcTimer repititions
CC = gcc
CFLAGS = -g -O0 -std=c99 -Wall -pedantic hw13.c -lpthread -o
VAL = valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./$(PROG) -t -f -s > $(MEM) 2>&1
SOURCE = hw13.c

#output files
MEM = mem.txt
OUT = out.txt

.SILENT:
.PHONY: all clean help mem test

all:$(PROG)

$(PROG):$(SOURCE)
	@echo "building your program"
	$(CC) $(CFLAGS) $(PROG)

test:$(PROG)
	@echo "running your test"

	@echo "running test one" >> $(OUT) 2>&1
	./$(PROG) -t 1 -s -v >> $(OUT) 2>&1
	@echo "running test two" >> $(OUT) 2>&1
	./$(PROG) -t 5 -s >> $(OUT) 2>&1
	@echo "running test three" >> $(OUT) 2>&1
	./$(PROG) -t 8 -v >> $(OUT) 2>&1
	@echo "running test four" >> $(OUT) 2>&1
	-./$(PROG) -t 0 >> $(OUT) 2>&1
	@echo "running test five" >> $(OUT) 2>&1
	-./$(PROG) -t 9 >> $(OUT) 2>&1

mem:$(PROG)
	@echo "running memory test"
	-$(VAL)

help:
	@echo "make options: all, clean help, mem, test"

clean:
	@echo "cleaning the current directory"
	rm -f $(OUT) $(MEM) *.o
	rm -f $(PROG)
