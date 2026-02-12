
CC = gcc

CFLAGS = -O2 -Wall -std=c99

LIBS = -lraylib -lm -lGL -lX11 -lpthread -ldl -lrt -lXrandr -lXinerama -lXcursor -lXxf86vm



TARGET = coulomb-sim



all: $(TARGET)



$(TARGET): main.c

	$(CC) $(CFLAGS) main.c -o $(TARGET) $(LIBS)



debug: CFLAGS += -g

debug: $(TARGET)



clean:

	rm -f $(TARGET) *.o



run: $(TARGET)

	./$(TARGET)



.PHONY: all clean debug run

