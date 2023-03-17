CC = gcc
CFLAGS = -Wall -Werror
LIBS = -lncurses

# Header files folder
HEADER_FILES_DIR=.

# Header file dir
INCLUDES=-I $(HEADER_FILES_DIR)

# Source file names
SRCS = binTree.c errorHandler.c input.c lexer.c main.c symbols.c syntax.c

# Header files
LIB_HEADERS=$(HEADER_FILES_DIR)/binTree.h defs.h errorHandler.h input.h lexer.h symbols.h syntax.h

# Object file names
OBJS = $(SRCS:.c=.o)

# Target executable name
TARGET = dCompiler

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)