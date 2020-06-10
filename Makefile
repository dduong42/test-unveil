.POSIX:
test-fork: test-fork.c
	clang -std=c99 -Wall -Wextra -o test-fork test-fork.c
