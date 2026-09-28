CC = gcc
CFLAGS = -Wall -Wextra -O2
SRC_DIR = src
INC_DIR = include

SRCS = $(SRC_DIR)/lexer.c $(SRC_DIR)/parser.c $(SRC_DIR)/codegen.c $(SRC_DIR)/main.c
OBJS = $(SRCS:.c=.o)
TARGET = c_compiler

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/%.h
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET) test_program.c