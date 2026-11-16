#include <signal.h>
#include <stdio.h>

#include "check.h"
#include "data_type/dynamic_arrays/char_array.h"
#include "macros/shell_globals.h"

// Static internal vars
static bool open_single_quotes;
static bool open_double_quotes;

static int open_parens;
static int open_braces;

static bool open_conds;
static bool open_loops;

static unsigned int unfinished_pipes;
static unsigned int unfinished_logic_and;
static bool seen_command_chars;

static bool open_backslashes;
static bool unfinished_backslash;

static unsigned int spaces_seen;

static CharArray *paren_brace_stack;

static CharArray *non_command_chars;

static bool do_errors = true;


// Static internal funcs
static void check_for_quotes(int chr) {
    if ((chr != '\"' && chr != '\'') || open_backslashes) {
        return;
    }

    if (chr == '\"' && !open_single_quotes) {
        open_double_quotes = !open_double_quotes;
    }
    if (chr == '\'' && !open_double_quotes) {
        open_single_quotes = !open_single_quotes;
    }
}

static void check_for_parens(int chr) {
    if (has_open_quotes()) {
        return;
    }

    if ((chr != '(' && chr != ')') || open_backslashes) {
        return;
    }

    if (chr == '(') {
        ++open_parens;

        if (!da_add_char(paren_brace_stack, ')')) {
            fprintf(stderr, "-%s: da_add error", SHELL_NAME);
            raise(SIGINT);
            return;
        }

        seen_command_chars = false;
    } else if (chr == ')') {
        --open_parens;

        char c = ')';
        if (da_count_char(paren_brace_stack) > 0 && !da_pop_char(paren_brace_stack, &c)) {
            fprintf(stderr, "-%s: da_pop error", SHELL_NAME);
            raise(SIGINT);
            return;
        }
        if (c != ')') {
            if (!da_add_char(paren_brace_stack, '}')) {
                fprintf(stderr, "-%s: da_add error", SHELL_NAME);
                raise(SIGINT);
                return;
            }

            if (do_errors) {
                fprintf(stderr, "-%s: expected '}' instead of ')'", SHELL_NAME);
                raise(SIGINT);
                return;
            }
        } else if (!seen_command_chars) {
            fprintf(stderr, "-%s: syntax error near unexpected token ')'", SHELL_NAME);
            raise(SIGINT);
            return;
        }
    }

    if (do_errors && open_parens < 0) {
        fprintf(stderr, "-%s: unexpected ')'", SHELL_NAME);
        raise(SIGINT);
        return;
    }
}

static void check_for_braces(int chr) {
    if (has_open_quotes()) {
        return;
    }

    if ((chr != '{' && chr != '}') || open_backslashes) {
        return;
    }

    if (chr == '{') {
        ++open_braces;

        if (!da_add_char(paren_brace_stack, '}')) {
            fprintf(stderr, "-%s: da_add error", SHELL_NAME);
            raise(SIGINT);
            return;
        }

        seen_command_chars = false;
    } else if (chr == '}') {
        --open_braces;

        char c = '}';
        if (da_count_char(paren_brace_stack) > 0 && !da_pop_char(paren_brace_stack, &c)) {
            fprintf(stderr, "-%s: da_pop error", SHELL_NAME);
            raise(SIGINT);
            return;
        }
        if (c != '}') {
            if (!da_add_char(paren_brace_stack, ')')) {
                fprintf(stderr, "-%s: da_add error", SHELL_NAME);
                raise(SIGINT);
                return;
            }

            if (do_errors) {
                fprintf(stderr, "-%s: expected ')' instead of '}'", SHELL_NAME);
                raise(SIGINT);
                return;
            }
        } else if (!seen_command_chars) {
            fprintf(stderr, "-%s: syntax error near unexpected token '}'", SHELL_NAME);
            raise(SIGINT);
            return;
        }
    }

    if (do_errors && open_braces < 0) {
        fprintf(stderr, "-%s: unexpected '}'", SHELL_NAME);
        raise(SIGINT);
        return;
    }
}

static void check_for_conds(int chr) { (void) chr; }

static void check_for_loops(int chr) { (void) chr; }

static void check_for_pipes_and_OR_logic_ops(int chr) {
    if (has_open_quotes() || open_backslashes || chr != '|') {
        if (do_errors && chr == '&' && unfinished_pipes != 0) {
            fprintf(stderr, "-%s: unexpected '&' near '|'", SHELL_NAME);
            raise(SIGINT);
            return;
        }

        if (chr != '\n' && chr != ' ') {
            unfinished_pipes = 0;
        }

        if (!has_open_quotes()) {
            if (chr == '\n' && !open_backslashes) {
                seen_command_chars = false;
            }

            if (!da_contains_char(non_command_chars, chr)) {
                seen_command_chars = true;
            }
        }

        if (chr == '|') {
            open_backslashes = false;
        }

        return;
    }

    if (do_errors && !seen_command_chars) {
        fprintf(stderr, "-%s: syntax error near token '|'", SHELL_NAME);
        raise(SIGINT);
        return;
    }

    if (do_errors && spaces_seen != 0 && unfinished_pipes != 0) {
        fprintf(stderr, "-%s: syntax error near token '|'", SHELL_NAME);
        raise(SIGINT);
        return;
    }

    ++unfinished_pipes;

    if (do_errors && unfinished_logic_and != 0) {
        fprintf(stderr, "-%s: unexpected '|' near '&'", SHELL_NAME);
        raise(SIGINT);
        return;
    }

    if (do_errors && unfinished_pipes == 3) {
        fprintf(stderr, "-%s: unexpected '|' near '||'", SHELL_NAME);
        raise(SIGINT);
        return;
    }
}

static void check_for_AND_logic_ops(int chr) {
    unsigned int prev_unfinished_logic_and = unfinished_logic_and;
    if (has_open_quotes() || open_backslashes || chr != '&') {
        if (chr != '\n' && chr != ' ') {
            unfinished_logic_and = 0;
        }

        if (!has_open_quotes()) {
            if (chr == '\n' && !open_backslashes) {
                seen_command_chars = false;
            }

            if (!da_contains_char(non_command_chars, chr)) {
                seen_command_chars = true;

                if (do_errors && prev_unfinished_logic_and == 1) {
                    fprintf(stderr, "-%s: unexpected '&'", SHELL_NAME);
                    raise(SIGINT);
                    return;
                }
            }
        }

        if (chr == '&') {
            open_backslashes = false;
        }

        return;
    }

    if (do_errors && !seen_command_chars) {
        fprintf(stderr, "-%s: syntax error near token '&'", SHELL_NAME);
        raise(SIGINT);
        return;
    }

    if (do_errors && spaces_seen != 0 && unfinished_logic_and != 0) {
        fprintf(stderr, "-%s: syntax error near token '&'", SHELL_NAME);
        raise(SIGINT);
        return;
    }

    ++unfinished_logic_and;

    if (do_errors && unfinished_logic_and == 3) {
        fprintf(stderr, "-%s: unexpected '&' near '&&'", SHELL_NAME);
        raise(SIGINT);
        return;
    }
}

static void check_for_backslash(int chr) {
    if (chr == '\\') {
        open_backslashes = !open_backslashes;
    } else if (chr != '\n') {
        open_backslashes = false;
    }

    unfinished_backslash = open_backslashes && chr == '\n';

    if (unfinished_backslash) {
        open_backslashes = false;
    }
}

static void check_for_spaces(int chr) {
    if (chr == ' ') {
        spaces_seen++;
    } else {
        spaces_seen = 0;
    }
}


// Funcs that check the static internal vars
void check_next_char(int chr) {
    // Quote check
    check_for_quotes(chr);

    // Paren check
    check_for_parens(chr);

    // Brace check
    check_for_braces(chr);

    // Conditional check
    check_for_conds(chr);

    // Loop check
    check_for_loops(chr);

    // Pipe check + logical OR check
    check_for_pipes_and_OR_logic_ops(chr);

    // AND logical operation check
    check_for_AND_logic_ops(chr);

    // Explicit backslash newline check
    check_for_backslash(chr);

    // Check spaces (error detection)
    check_for_spaces(chr);
}

void line_check_reset(void) {
    open_single_quotes = false;
    open_double_quotes = false;

    open_parens = 0;
    open_braces = 0;

    open_conds = false;
    open_loops = false;

    unfinished_pipes = 0;
    unfinished_logic_and = 0;
    seen_command_chars = false;

    open_backslashes = false;
    unfinished_backslash = false;

    spaces_seen = 0;

    da_free_char(paren_brace_stack);
    paren_brace_stack = da_new_char();

    if (non_command_chars == NULL) {
        non_command_chars = da_new_char();

        da_add_char(non_command_chars, '(');
        // da_add_char(non_command_chars, ')');
        da_add_char(non_command_chars, '{');
        // da_add_char(non_command_chars, '}');
        da_add_char(non_command_chars, '|');
        da_add_char(non_command_chars, '&');
        da_add_char(non_command_chars, ' ');
        da_add_char(non_command_chars, '\n');

        if (da_count_char(non_command_chars) != 6) {
            fprintf(stderr, "-%s: da_add error", SHELL_NAME);
            raise(SIGINT);
            return;
        }
    }

    do_errors = true;
}

void set_do_error(bool change) { do_errors = change; }

void clean_up_check(void) {
    da_free_char(paren_brace_stack);
    da_free_char(non_command_chars);
}

bool is_complete_command(void) {
    if (has_open_quotes()) {
        return false;
    }

    if (has_open_parens()) {
        return false;
    }

    if (has_open_braces()) {
        return false;
    }

    if (has_open_conditional()) {
        return false;
    }

    if (has_open_loop()) {
        return false;
    }

    if (has_unfinished_pipe()) {
        return false;
    }

    if (has_unfinished_logical_operator()) {
        return false;
    }

    if (has_unfinished_backslash()) {
        return false;
    }

    return true;
}

bool has_open_quotes(void) { return open_single_quotes || open_double_quotes; }

bool has_open_parens(void) { return open_parens > 0; }

bool has_open_braces(void) { return open_braces > 0; }

bool has_open_conditional(void) { return open_conds; }

bool has_open_loop(void) { return open_loops; }

bool has_unfinished_pipe(void) { return unfinished_pipes == 1; }

bool has_unfinished_logical_operator(void) { return unfinished_pipes == 2 || unfinished_logic_and == 2; }

bool has_unfinished_backslash(void) { return unfinished_backslash; }

bool has_open_backslash(void) { return open_backslashes; }
