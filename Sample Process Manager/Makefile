# C compiler & flags
CC=gcc
CFLAGS = -Wall -g
LIB=-lm

# Define set implementation of source & object file
SOURCE = main.c processQueue.c simulatedMem.c processManager.c
OBJ=$(SOURCE:.c=.o)
EXE=allocate

# For procecessManager.c compilation to .o
PROC_MANAGER_P1 = processManager.c processManager.h
PROC_MANAGER_P2 = simulatedMem.h processQueue.h

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ) $(LIB)

main.o: main.c usefulConsts.h processManager.h
	$(CC) $(CFLAGS) -c main.c

processQueue.o: processQueue.c simulatedMem.h processQueue.h usefulConsts.h 
	$(CC) $(CFLAGS) -c processQueue.c

simulatedMem.o: simulatedMem.c simulatedMem.h usefulConsts.h
	$(CC) $(CFLAGS) -c simulatedMem.c

processManager.o:  $(PROC_MANAGER_P1) $(PROC_MANAGER_P2)
	$(CC) $(CFLAGS) -c processManager.c

clean:
	rm -f $(OBJ) $(EXE)

