CC = gcc
CFLAGS = -Iinclude -std=c11 -Wall
TARGET = iot_keymgr
SRCS = src/device.c src/key_manager.c src/logger.c src/main.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET).exe $(SRCS)

demo: $(TARGET)
	./$(TARGET).exe --demo

clean:
	rm -f $(TARGET).exe logs/*.log