CC = gcc
PROGNAME = server-http
CFLAGS = -Wall -g -I./src
CFLAGS_TEST = -Wall -g -I./test
SRC_DIR = src
OBJ_DIR = build
TEST_DIR = test
C_FIND = $(shell find $(SRC_DIR) -name '*.c')
OBJ = $(SRC_DIR)/network.o $(SRC_DIR)/main.o $(SRC_DIR)/request.o $(SRC_DIR)/data-struct/queue.o $(SRC_DIR)/data-struct/linkedList.o $(SRC_DIR)/malloc-utils/malloc_utils.o $(SRC_DIR)/data-struct/dictionary.o

#deps use the dep command to generate this
main.o: $(SRC_DIR)/main.c $(SRC_DIR)/request.h $(SRC_DIR)/network.h
request.o: src/request.c $(SRC_DIR)/request.h $(SRC_DIR)/data-struct/dictionary.h $(SRC_DIR)/data-struct/queue.h $(SRC_DIR)/data-struct/linkedList.h
network.o: $(SRC_DIR)/network.c $(SRC_DIR)/network.h
test.o: $(TEST_DIR)/network-test.c
malloc_utils.o: $(SRC_DIR)/malloc-utils/malloc_utils.c $(SRC_DIR)/malloc-utils/malloc_utils.h
queue.o: $(SRC_DIR)/data-struct/queue.c $(SRC_DIR)/data-struct/queue.h $(SRC_DIR)/data-struct/linkedList.h $(SRC_DIR)/malloc-utils/malloc_utils.h
linkedList.o: $(SRC_DIR)/data-struct/linkedList.c $(SRC_DIR)/data-struct/linkedList.h  $(SRC_DIR)/malloc-utils/malloc_utils.h
malloc_utils.o: $(SRC_DIR)/malloc-utils/malloc_utils.c $(SRC_DIR)/malloc-utils/malloc_utils.h
dictionary.o: $(SRC_DIR)/data-struct/dictionary.c $(SRC_DIR)/data-struct/dictionary.h $(SRC_DIR)/data-struc ../malloc-utils/malloc_utils.h

all:
server-http: $(OBJ)
	$(CC) $(CFLAGS) -o	$(PROGNAME) $(OBJ)

dep:
	@echo "dependecy generacio"
	$(CC) -MM  $(C_FIND)

clean:
	rm -f $(PROGNAME) $(SRC_DIR)/*.o*.o

test:
	$(CC) $(CFLAGS_TEST) -o test $(test.o)
