CC = gcc

CFLAGS = -IUnity/src

TARGET = test_serial_rpc.exe

SOURCES = test/test_serial_rpc.c \
          Unity/src/unity.c \
          serial_rpc.c

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -rf $(TARGET)

run:
	$(TARGET)
