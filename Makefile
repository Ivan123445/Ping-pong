CC = g++

TARGET = pong
TARGET_PREF = build/

FLAGS = -Wall -Wextra -Werror

LIBS = -lncurses

SCRS = \
	src/pong.cpp \
	src/classes.cpp

all: clean configure rebuild

configure:
	@if ! [ -d ./build/ ]; then \
	mkdir "build"; \
	fi

rebuild:
	$(CC) $(FLAGS) $(SCRS) $(LIBS) -o $(TARGET_PREF)$(TARGET)

clean:
	rm -rf build/*
