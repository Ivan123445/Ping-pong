CC = gcc

TARGET = PROG
TARGET_SBS = PROG_SBS
TARGET_PREF = build/

FLAGS = -Wall -Wextra -Werror

LIBS = -lncurses

SCRS = \
	src/pong.c

SCRS_SBS = \
	src/pong_step_by_step.c


all: clean configure rebuild_sbs

configure:
	@if ! [ -d ./build/ ]; then \
	mkdir "build"; \
	fi

rebuild:
	$(CC) $(FLAGS) $(SCRS) $(LIBS) -o $(TARGET_PREF)$(TARGET)

rebuild_sbs:
	$(CC) $(FLAGS) $(SCRS_SBS) -o $(TARGET_PREF)$(TARGET_SBS)

clean:
	rm -rf build/*
