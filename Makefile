CC=gcc
CFLAGS=
LIBS=-lX11
OBJS_MAIN=main.cpp
EJS=main

.PHONY: all
all: $(EJS)

main: $(OBJS_MAIN)
	rm -rf output && mkdir output && $(CC) $(CFLAGS) -o output/center_window $(OBJS_MAIN) $(LIBS) 

clean:
	rm -f output/center_window

install:
	cp output/center_window /usr/local/bin