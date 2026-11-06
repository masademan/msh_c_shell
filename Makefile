#  Vars
# CFLAGS = -std=c11 -Wall -Wextra -O0 -pedantic -g
BASE_CFLAGS = -std=gnu11 -Wall -Wextra -pedantic
FAST_FLAGS = -O2
NORMAL_FLAGS = -O0 -g
CFLAGS = $(BASE_CFLAGS) $(NORMAL_FLAGS)

CC = gcc

ASAN_FLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all \
					 --track-origins=yes --error-exitcode=1

FAST_MSG = ""

SHELL_NAME = msh

#  Parameters
m ?= Default message from Makefile commit


all: $(SHELL_NAME)

fast: CFLAGS = $(BASE_CFLAGS) $(FAST_FLAGS)
fast: FAST_MSG = " - Compiled with optimization turned on (-O2)\n"
fast: $(SHELL_NAME)

asan: CFLAGS += $(ASAN_FLAGS)
asan: $(SHELL_NAME)

$(SHELL_NAME): main.o \
	shell_loop.o shell_read_line.o shell_parse_line.o shell_execute.o \
	get_sysinfo.o shell_helpers.o
	@printf "\nLinking the files to finish compiling...\n\n"
	@$(CC) $(CFLAGS) $^ -o $@
	@printf "Finished!\n"
	@printf $(FAST_MSG)

%.o: %.c
	@printf "Compiling $<...\n"
	@$(CC) $(CFLAGS) -c $<

clean:
	@printf "Cleaning up files\n"
	@rm -f *.o $(SHELL_NAME)

commit:
	@printf "Commiting changes\n"
	@git add .
	@git commit -m "$(m)"

push:
	@git push origin main

pull:
	@git pull origin main

.PHONY: all clean commit push pull fast asan
