#programs
PROG = hw14
PLOT = myplot2
GEN = genErrors

CC = gcc
CFLAGS = -g -std=c99 $(SOURCE) -lm -o
SOURCE = hw14.c ODEsolvers.c simulations.c

VAL = valgrind --tool=memcheck --leak-check=yes --track-origins=yes

#simfiles/png creation
T = t
S = s
TANK = tank
SPRING = spring
DAMP = damp
DIFF = diff

#output files
MEM = mem.txt
OUT = out.txt

.SILENT:
.PHONY: all clean help mem test

all:$(PROG)

$(PROG):$(SOURCE)
	@echo "building your program"
	$(CC) $(CFLAGS) $(PROG)

damp: $(PROG)
	echo "running damp"
	./$(PROG) -spring -rk2 -step .01 -ftime 10 -x1 1 -x2 0 -damp .2 > $(DAMP).sim
	./$(PLOT) -damp $(DAMP).sim $(DAMP).png

spring:$(PROG)
	echo "running spring"
	./$(PROG) -spring -x1 1 -x2 0 -eu -step .01 -ftime 10 > $(S)1.sim
	./$(PROG) -spring -x1 1 -x2 0 -rk2 -step .01 -ftime 10 > $(S)2.sim
	./$(PROG) -spring -x1 1 -x2 0 -rk3 -step .01 -ftime 10 > $(S)3.sim
	./$(PROG) -spring -x1 1 -x2 0 -rk4 -step .01 -ftime 10 > $(S)4.sim
	paste $(S)1.sim $(S)2.sim $(S)3.sim $(S)4.sim > $(SPRING).sim
	./$(GEN) $(SPRING).sim $(DIFF).sim
	./$(PLOT) -spring $(DIFF).sim $(DIFF).png

tank:$(PROG)
	echo "running tank"
	./$(PROG) -tank -x1 0 -x2 0 -eu -step 5 -ftime 600 > $(T)1.sim
	./$(PROG) -tank -x1 0 -x2 0 -rk2 -step 5 -ftime 600 > $(T)2.sim
	./$(PROG) -tank -x1 0 -x2 0 -rk3 -step 5 -ftime 600 > $(T)3.sim
	./$(PROG) -tank -x1 0 -x2 0 -rk4 -step 5 -ftime 600 > $(T)4.sim
	paste $(T)1.sim $(T)2.sim $(T)3.sim $(T)4.sim > $(TANK).sim
	./$(PLOT) -tank $(TANK).sim $(TANK).png

test:$(PROG)
	echo "running test"
	make damp
	make tank 
	make spring

mem:$(PROG)
	@echo "running memory test"
	echo "running rk3 mem test..." >> $(MEM)
	$(VAL) ./$(PROG) -tank -x1 0 -x2 0 -rk3 -step 5 -ftime 600 >> $(MEM) 2>&1
	echo "running rk4 mem test..." >> $(MEM)
	$(VAL) ./$(PROG) -tank -x1 0 -x2 0 -rk4 -step 5 -ftime 600 >> $(MEM) 2>&1

help:
	@echo "make options: all, clean, damp, help, mem, spring, tank, test"

clean:
	@echo "cleaning the current directory"
	rm -f *.sim *.png
	rm -f $(PROG) $(MEM)
