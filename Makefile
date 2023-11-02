cflags = -g -Wall -Wextra -fsanitize=address,undefined
files = $(shell find ./src -type f -name "*.c")
lflags = -I./src/

main:
	clang $(cflags) $(files) -o ./prog $(lflags)