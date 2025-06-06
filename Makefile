CC=gcc
CFLAGS=-Wall -Wextra -O2
TARGET=http_server

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $(TARGET) main.c

clean:
	rm -f $(TARGET)

