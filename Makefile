CFLAGS = -std=c99 -Wall -Wextra -O0 -pedantic -g
CC = gcc

ASAN_FLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all \
					 --track-origins=yes --error-exitcode=1

all: msh

msh: main.o shell_loop.o shell_read_line.o shell_parse_line.o shell_execute.o get_dir.o
	@printf "\nLinking the files to finish compiling\n\n"
	@$(CC) $(CFLAGS) $^ -o $@
#	@chmod +x $@
	@printf "Finished!\n"

%.o: %.c
	@printf "Compiling $<...\n"
	@$(CC) $(CFLAGS) -c $<

clean:
	@printf "Cleaning up files\n"
	@rm -f *.o msh

.PHONY: all clean
