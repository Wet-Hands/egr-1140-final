CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11
TARGET  = dungeon_doom.exe
SRCS    = main.c room.c move.c ui.c save.c
OBJS    = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /Q $(OBJS) $(TARGET) 2>nul || true

.PHONY: all clean
