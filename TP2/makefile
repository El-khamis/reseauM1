CC = gcc
CFLAGS = -Wall
EXEC_NAME = Exo2-1
OBJ_FILES = Exo2-1.o

all : $(EXEC_NAME)

clean :
	rm $(EXEC_NAME) $(OBJ_FILES)

$(EXEC_NAME) : $(OBJ_FILES)
	$(CC) -pthread -o $(EXEC_NAME) $(OBJ_FILES)

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $<
