CFLAGS = -std=c11 -Wall -Wextra -O0 -pedantic -g
CC = gcc

ASAN_FLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all \
					 --track-origins=yes --error-exitcode=1

m ?= Default message from Makefile commit

all: msh

msh: main.o shell_loop.o shell_read_line.o shell_parse_line.o shell_execute.o get_sysinfo.o
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

commit:
	@printf "Commiting changes\n"
	@git add .
	@git commit -m "$(m)"

push:
	@git push origin main

pull:
	@git pull origin main

.PHONY: all clean commit push pull
