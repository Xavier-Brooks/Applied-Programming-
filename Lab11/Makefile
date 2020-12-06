#compiler and valgrind arguments
PROG = Hw11
PROG1 = correction

#helper binaries
PLOT = myplot
ERROR = detError
DEVICE = realDevice
PASTE = paste
VAL = diffVal
EXEC = chmod

#compilation flags
CC = gcc
CFLAGS = -g hw11.c DynamicArrays.c -DHW8 -std=c99 -lgsl -lgslcblas
CCFLAGS = -g correction.c -DHW8 -std=c99 -o
VALGRIND = valgrind --tool=memcheck --leak-check=yes --track-origins=yes

#binary parameters
HPARAMS = -order 3 -p

#input files
DATA = data.txt

#output files
MEM = mem.txt
REAL = realdevices.txt
CORRECT = correction.txt
ALL = alldata.txt
PIC = correction.png

.SILENT:
.PHONY: all base correct clean help mem plot x

$(PROG):
	$(CC) $(CFLAGS) -o $(PROG)

$(PROG1):
	$(CC) $(CCFLAGS) $(PROG1) -lm

all: $(PROG) $(PROG1)
	@echo "compiling your binaries"

base:
	./$(DEVICE) | ./$(ERROR)

correct:$(PROG1)
	./$(DEVICE) | ./$(PROG1) | ./$(ERROR)

mem:$(PROG)
	@echo "running memory test"
	$(VALGRIND) ./$(PROG) $(HPARAMS) $(DATA)

plot:$(PROG1)
	@echo "forming and plotting your data"
	./$(DEVICE) >> $(REAL)
	./$(DEVICE) | ./$(PROG1) >> $(CORRECT)
	$(PASTE) $(REAL) $(CORRECT) > $(ALL)
	./$(PLOT) -i $(ALL) -o $(PIC)

x:
	$(EXEC) +x $(DEVICE) $(ERROR) $(PLOT) $(VAL)

help:
	@echo "make options: all, base, correct, clean, help, mem, test"

clean:
	@echo "cleaning the current directory"
	rm -f $(REAL) $(MEM) $(CORRECT) $(ALL) $(PIC)
	rm -f $(PROG) $(PROG1)
	$(EXEC) -x $(DEVICE) $(ERROR) $(VAL) $(PLOT)

