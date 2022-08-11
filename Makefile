CC     := g++
CFLAGS := -Wall -Wpedantic -Wextra -Werror -Wno-deprecated-enum-enum-conversion -Ofast -fopenmp -DNDEBUG -std=c++20 -I/usr/local/include/opencv4
LIBS   := -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_videoio
RM     := rm -rf

PROGS = main

.PHONY: all

all: $(PROGS)

main: main.cpp
	@echo Generating: $@
	$(CC) $(CFLAGS) $(LIBS) -o $@ $<

clean:
	$(RM) $(PROGS)
