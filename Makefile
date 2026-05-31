CC= g++
CFLAGS = -lSDL3 -lGL -lGLEW -lglm 
TARGET = main

SRCS = src/*.cpp vendor/*.cpp

all: $(TARGET)

$(TARGET) : $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)
