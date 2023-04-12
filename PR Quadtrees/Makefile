# C compiler & Flags
CC = gcc
CFLAGS = -Wall -g

# Define library to be linked to
LIB=

# Define set implementation of source & object file
SOURCE_PART1 = main.c linkedList.c footpathData.c dataPoint.c point2D.c 
SOURCE_PART2 = quadTree.c rectangle.c
SOURCE = $(SOURCE_PART1) $(SOURCE_PART2) 
OBJ=$(SOURCE:.c=.o)


# For quadtree.c compilation to .o
QUAD_TREE_P1= quadTree.c quadTree.h rectangle.h dataPoint.h 
QUAD_TREE_P2= footpathData.h usefulConsts.h


# executable names
EXE1=pointSearcher
EXE2=regionSearcher

$(EXE1): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXE1) $(OBJ) $(LIB)

$(EXE2): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXE2) $(OBJ) $(LIB)

main.o: main.c point2D.h footpathData.h linkedList.h quadTree.h rectangle.h
	$(CC) $(CFLAGS) -c main.c

footpathData.o: footpathData.c footpathData.h point2D.h usefulConsts.h
	$(CC) $(CFLAGS) -c footpathData.c

linkedList.o: linkedList.c linkedList.h footpathData.h usefulConsts.h
	$(CC) $(CFLAGS) -c linkedList.c

quadTree.o: $(QUAD_TREE_P1) $(QUAD_TREE_P2)
	$(CC) $(CFLAGS) -c quadTree.c

point2D.o: point2D.c point2D.h usefulConsts.h
	$(CC) $(CFLAGS) -c point2D.c

dataPoint.o: dataPoint.c dataPoint.h point2D.h usefulConsts.h footpathData.h
	$(CC) $(CFLAGS) -c dataPoint.c

rectangle.o: rectangle.c rectangle.h point2D.h usefulConsts.h
	$(CC) $(CFLAGS) -c rectangle.c

clean:
	rm -f $(OBJ) $(EXE1) $(EXE2)