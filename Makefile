CC = gcc
CFLAGS = -O2 -Wall -std=c99 `pkg-config --cflags raylib 2>/dev/null || echo ""`
LIBS = `pkg-config --libs raylib 2>/dev/null || echo "-lraylib -lm -lGL -lX11 -lpthread -ldl -lrt -lXrandr -lXinerama -lXcursor"`

TARGET = coulomb-sim

all: $(TARGET)

$(TARGET): charges.c
	$(CC) $(CFLAGS) charges.c -o $(TARGET) $(LIBS)

debug: CFLAGS += -g
debug: $(TARGET)

clean:
	rm -f $(TARGET) *.o

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean debug run
