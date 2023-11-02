cflags = -Wall -Wextra
files = $(shell find . -type f -name "*.c")
lflags = -I./src/

main:
	clang $(cflags) $(files) -o ./prog $(lflags)