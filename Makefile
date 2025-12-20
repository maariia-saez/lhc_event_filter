# Complier and Flags
CC = gcc
# Strict flags for reliability (ANSI C compliance)
CFLAGS = -Wall -Werror -Wextra -ansi -pedantic -Iinclude

# Targets
all: bin/generator bin/analyzer

# --- 1. LINKING ---
bin/analyzer: obj/analyzer.o obj/events.o
	@mkdir -p bin
	$(CC) $(CFLAGS) -o bin/analyzer obj/analyzer.o obj/events.o

bin/generator: obj/generator.o
	@mkdir -p bin
	$(CC) $(CFLAGS) -o bin/generator obj/generator.o

# --- COMPILATION ---
obj/analyzer.o: src/analyzer.c include/events.h
	@mkdir -p obj
	$(CC) $(CFLAGS) -c src/analyzer.c -o obj/analyzer.o

obj/events.o: src/events.c include/events.h
	@mkdir -p obj
	$(CC) $(CFLAGS) -c src/events.c -o obj/events.o

obj/generator.o: src/generator.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -c src/generator.c -o obj/generator.o

# CLean Up
clean:
	rm -rf obj bin data/*.dat
	@echo "Cleaned buikd artifacts."
