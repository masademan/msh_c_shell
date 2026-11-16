#  Vars
# BASE_CFLAGS = -std=c11 -Wall -Wextra -O0 -pedantic -g
BASE_CFLAGS = -std=gnu11 -Wall -Wextra -pedantic -I.
FAST_FLAGS = -O2
NORMAL_FLAGS = -O0 -g
CFLAGS ?= $(BASE_CFLAGS) $(NORMAL_FLAGS)

CC = gcc

ASAN_FLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all \
					 --track-origins=yes --error-exitcode=1

SHELL_NAME = msh

SRCS = main.c \
			 shell/loop.c shell/read_command.c shell/parse_command.c shell/execute.c \
			 shell/helpers/helpers.c \
			 shell/helpers/commands/check.c shell/helpers/commands/edit.c shell/helpers/commands/move.c \
			 shell/helpers/commands/clean.c shell/helpers/commands/split.c \
			 shell/helpers/arguments/clean.c shell/helpers/arguments/split.c \
			 shell/history/history.c \
			 get_info/get_sysinfo.c \
			 data_type/dynamic_arrays/char_array.c data_type/dynamic_arrays/string_array.c \
			 data_type/dynamic_arrays/char_array_and_string_conversion.c \
			 data_type/safe_string/safe_string.c

OBJS = $(SRCS:%.c=objs/%.o)


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
	@printf "\0--- Cleaning old .o files ---\n"
	@$(MAKE) --no-print-directory clean
	@printf "\n\n--- Compiling ---\n"
	@$(MAKE) --no-print-directory $(SHELL_NAME)
	@printf "\n\n--- Cleaning .o files ---\n"
	@$(MAKE) --no-print-directory clean

trial:
	@printf "\0--- Compiling '$(SHELL_NAME)' ---\n"
	@$(MAKE) --no-print-directory $(SHELL_NAME)
	@printf "\n\n--- Running '$(SHELL_NAME)' to test ---\n\n"
	./$(SHELL_NAME)

fast:
	@printf "\0--- Clearing out old .o files to prevent mixing ---\n"
	@$(MAKE) --no-print-directory clean
	@printf "\n\n--- Compiling with optimization ---\n"
	@$(MAKE) --no-print-directory $(SHELL_NAME) CFLAGS="$(BASE_CFLAGS) $(FAST_FLAGS)" \
		MSG="' - Compiled with optimization turned on (-O2)\n'"
	@printf "\n\n--- Clearing out new .o files to prevent mixing ---\n"
	@$(MAKE) --no-print-directory clean_objs
	@printf "Finished!\n"

asan:
	@printf "\0--- Clearing out old .o files to prevent mixing ---\n"
	@$(MAKE) --no-print-directory clean
	@printf "\n\n--- Compiling with ASAN ---\n"
	@$(MAKE) --no-print-directory $(SHELL_NAME) \
		CFLAGS="$(CFLAGS) $(ASAN_FLAGS)" MSG="' - Compiled with ASAN\n'"
	@printf "\n\n--- Running '$(SHELL_NAME)' with ASAN ---\n\n"
	./$(SHELL_NAME)
	@printf "\n\n--- Cleaning ASAN files to prevent mixing in future tests ---\n"
	@$(MAKE) --no-print-directory clean

valgrind:
	@printf "\0--- Clearing out old .o files to prevent mixing ---\n"
	@$(MAKE) --no-print-directory clean
	@printf "\n\n--- Compiling normally ---\n"
	@$(MAKE) --no-print-directory $(SHELL_NAME)
	@printf "\n\n--- Running '$(SHELL_NAME)' with Valgrind ---\n\n"
	$(VALGRIND) ./$(SHELL_NAME)

$(SHELL_NAME): $(OBJS)
	@printf "\nLinking the files to finish compiling...\n\n"
	@$(CC) $(CFLAGS) $^ -o $@
	@printf "Finished!\n"
	@printf $(MSG)

objs/%.o: %.c
	@printf "Compiling $<...\n"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@printf "Cleaning up files...\n"
	@$(MAKE) --no-print-directory clean_objs
	@$(MAKE) --no-print-directory clean_shell
	@printf "\nFinished!\n"

clean_objs:
	@rm -rf objs/
	@#rm -f *.o
	@#find . -name "*.o" -type f -delete

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

count:
	@printf "Counting lines of code...\n"
	@cloc get_info/ macros/ shell/ Makefile main.c


.PHONY: all clean clean_objs clean_shell commit push pull check fast trial asan valgrind count
