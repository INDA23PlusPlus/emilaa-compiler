cflags = -g -Wall -Wextra -Werror
files = $(shell find . -type f -name "*.c")

main:
	clang $(cflags) $(files) -o ./prog