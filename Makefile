CC = gcc
PROGNAME = server-http
CFLAGS = -Wall -g -I./src
SRC_DIR = src
OBJ_DIR = build

OBJ = $(SRC_DIR)/network.o $(SRC_DIR)/main.o $(SRC_DIR)/request.o

#deps use the dep command to generate this
main.o: $(SRC_DIR)/main.c $(SRC_DIR)/request.h $(SRC_DIR)/network.h
request.o: $(SRC_DIR)/request.c $(SRC_DIR)/request.h
network.o: $(SRC_DIR)/network.c $(SRC_DIR)/network.c

all:

server-http: $(OBJ)
	$(CC) $(CFLAGS) -o	$(PROGNAME) $(OBJ)

dep:
	$(CC) -MM $(SRC_DIR)/*.c

clean:
	rm -f $(PROGNAME) $(SRC_DIR)/*.o*.o
