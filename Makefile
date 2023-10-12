cflags = -g -Wall -Wextra -Werror
files = $(shell find . -type f -name "*.cpp")

main:
	clang++ $(cflags) $(files) -o ./prog