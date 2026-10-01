CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -O2
LDFLAGS = -lcrypto

SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:.c=.o)
TARGET = merkleTree

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
