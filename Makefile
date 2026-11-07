#  Vars
# BASE_CFLAGS = -std=c11 -Wall -Wextra -O0 -pedantic -g
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

# Makefile docs:
# "make" / "make msh":
#   makes the executable shell (run with "./msh")
# "make clean":
#   cleans all the .o files as well as the msh executable
# "make fast":
#   just like "make msh", but uses -O2 as a flag in gcc
# "make trial":
#   just like "make msh", but also runs it after compiling
# "make asan":
#   just like "make msh", but uses ASAN flags and runs it after compiling
#    (used for memory leak checks)
# "make valgrind":
#   just like "make msh", but runs it using valgrind after compiling
#    (used for memory leak checks)
# "make commit":
#   commits all the files in the directory with a message
#    the message can be changed with 'm=""' ('make commit m="[msg here]"')
# "make push":
#   pushes changes to the main branch in the linked github repo
# "make pull":
#   pulls from the main branch in the linked github repo


all: $(SHELL_NAME)

check:
	@printf "\0--- Deleting old .o files ---\n"
	@$(MAKE) --no-print-directory clean
	@printf "\n--- Compiling ---\n"
	@$(MAKE) --no-print-directory $(SHELL_NAME)
	@printf "\n--- Deleting old .o files ---\n"
	@$(MAKE) --no-print-directory clean

trial: $(SHELL_NAME)
	@printf "\n--- Running '$(SHELL_NAME)' to test ---\n\n"
	./$(SHELL_NAME)

fast:
	@printf "Clearing out old .o files to prevent mixing...\n"
	@$(MAKE) --no-print-directory clean
	@printf "\nCompiling with optimization:\n"
	@$(MAKE) --no-print-directory $(SHELL_NAME) CFLAGS="$(BASE_CFLAGS) $(FAST_FLAGS)" \
		MSG="' - Compiled with optimization turned on (-O2)\n'"
	@printf "Clearing out new .o files to prevent mixing...\n"
	@$(MAKE) --no-print-directory clean_objs

asan:
	@printf "Clearing out old .o files to prevent mixing...\n"
	@$(MAKE) --no-print-directory clean
	@printf "\nCompiling with ASAN:\n"
	@$(MAKE) --no-print-directory $(SHELL_NAME) \
		CFLAGS="$(CFLAGS) $(ASAN_FLAGS)" MSG="' - Compiled with ASAN'"
	@printf "\n\n--- Running '$(SHELL_NAME)' with ASAN ---\n\n"
	./$(SHELL_NAME)
	@printf "\nCleaning ASAN files to prevent mixing in future tests...\n"
	@$(MAKE) --no-print-directory clean

valgrind:
	@printf "Clearing out old .o files to prevent mixing...\n"
	@$(MAKE) --no-print-directory clean
	@printf "\nCompiling normally:\n"
	@$(MAKE) --no-print-directory $(SHELL_NAME)
	@printf "\n\n--- Running '$(SHELL_NAME)' with Valgrind ---\n\n"
	$(VALGRIND) ./$(SHELL_NAME)

$(SHELL_NAME): main.o \
	shell_loop.o shell_read_command.o shell_parse_command.o shell_execute.o \
	get_sysinfo.o shell_helpers.o shell_command_check.o
	@printf "\nLinking the files to finish compiling...\n\n"
	@$(CC) $(CFLAGS) $^ -o $@
	@printf "Finished!\n"
	@printf $(MSG)

%.o: %.c
	@printf "Compiling $<...\n"
	@$(CC) $(CFLAGS) -c $<

clean:
	@printf "Cleaning up files...\n"
	@$(MAKE) --no-print-directory clean_objs
	@$(MAKE) --no-print-directory clean_shell
	@printf "\nFinished!\n"

clean_objs:
	@rm -f *.o

clean_shell:
	@rm -f $(SHELL_NAME)

commit:
	@printf "Commiting changes...\n"
	@git add .
	@git commit -m "$(m)"

push:
	@printf "Pushing to main branch...\n"
	@git push origin main

pull:
	@printf "Pulling from main branch...\n"
	@git pull origin main


.PHONY: all clean clean_objs clean_shell commit push pull check fast trial asan valgrind
