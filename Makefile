CCFLAGS=-Wall -Wextra -pedantic -std=c99 -O2 -g
CC=gcc
LIBS=
OBJS_MAIN=src/main.c
EJS=main

ifeq ($(OS),Windows_NT)
    CCFLAGS += -D _WIN32
    ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
        CCFLAGS += -D AMD64
    else
        ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
            CCFLAGS += -D AMD64
        endif
        ifeq ($(PROCESSOR_ARCHITECTURE),x86)
            CCFLAGS += -D IA32
        endif
    endif
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        CCFLAGS += -D LINUX
		LIBS+=-lX11
    endif
    ifeq ($(UNAME_S),Darwin)
        CCFLAGS += -D OSX
    endif
    UNAME_P := $(shell uname -p)
    ifeq ($(UNAME_P),x86_64)
        CCFLAGS += -D AMD64
    endif
    ifneq ($(filter %86,$(UNAME_P)),)
        CCFLAGS += -D IA32
    endif
    ifneq ($(filter arm%,$(UNAME_P)),)
        CCFLAGS += -D ARM
    endif
endif

.PHONY: all
all: $(EJS)

main: $(OBJS_MAIN)
	rm -rf output && mkdir output && $(CC) $(CCFLAGS) -o output/move_focused_window $(OBJS_MAIN) $(LIBS) 

clean:
	rm -f output/move_focused_window

install:
	cp output/move_focused_window /usr/local/bin

uninstall:
	rm -rf /usr/local/bin/move_focused_window
