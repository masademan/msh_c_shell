#  Vars
# CFLAGS = -std=c11 -Wall -Wextra -O0 -pedantic -g
BASE_CFLAGS = -std=gnu11 -Wall -Wextra -pedantic
FAST_FLAGS = -O2
NORMAL_FLAGS = -O0 -g
CFLAGS ?= $(BASE_CFLAGS) $(NORMAL_FLAGS)

CC = gcc

ASAN_FLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all \
					 --track-origins=yes --error-exitcode=1

SHELL_NAME = msh

#  Parameters
MSG ?= ""
m ?= Default message from Makefile commit


all: $(SHELL_NAME)

trial: $(SHELL_NAME)
	@printf "\n--- Running '$(SHELL_NAME)' to test ---\n\n"
	./$(SHELL_NAME)

fast:
	@printf "Clearing out old .o files to prevent mixing...\n"
	@$(MAKE) --no-print-directory clean
	@printf "\nCompiling with optimization:\n"
	@$(MAKE) --no-print-directory $(SHELL_NAME) CFLAGS="$(BASE_CFLAGS) $(FAST_FLAGS)" \
		MSG="' - Compiled with optimization turned on (-O2)\n'"

asan:
	@printf "Clearing out old .o files to prevent mixing...\n"
	@$(MAKE) --no-print-directory clean
	@printf "\nCompiling with ASAN:\n"
	@$(MAKE) --no-print-directory $(SHELL_NAME) \
		CFLAGS="$(CFLAGS) $(ASAN_FLAGS)" MSG="' - Compiled with ASAN'"
	@printf "\n\n--- Running '$(SHELL_NAME)' with ASAN ---\n\n"
	./$(SHELL_NAME)

valgrind:
	@printf "Clearing out old .o files to prevent mixing...\n"
	@$(MAKE) --no-print-directory clean
	@printf "\nCompiling normally:\n"
	@$(MAKE) --no-print-directory $(SHELL_NAME)
	@printf "\n\n--- Running '$(SHELL_NAME)' with Valgrind ---\n\n"
	$(VALGRIND) ./$(SHELL_NAME)

$(SHELL_NAME): main.o \
	shell_loop.o shell_read_line.o shell_parse_line.o shell_execute.o \
	get_sysinfo.o shell_helpers.o
	@printf "\nLinking the files to finish compiling...\n\n"
	@$(CC) $(CFLAGS) $^ -o $@
	@printf "Finished!\n"
	@printf $(MSG)

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

.PHONY: all clean commit push pull fast trial asan valgrind
