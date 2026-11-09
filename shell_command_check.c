#include <signal.h>
#include <stdio.h>

#include "shell_command_check.h"
#include "shell_globals.h"

// Static internal vars
static bool open_single_quotes;
static bool open_double_quotes;

static int open_parens;
static int open_braces;

static bool open_conds;
static bool open_loops;

static unsigned int unfinished_pipes;
static unsigned int unfinished_logic_and;

static bool open_backslashes;
static bool unfinished_backslash;


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

    if (chr != '(' && chr != ')') {
        return;
    }

    if (chr == '(') {
        ++open_parens;
    } else if (chr == ')') {
        --open_parens;
    }

    if (open_parens < 0) {
        fprintf(stderr, "-%s: unexpected ')'", SHELL_NAME);
        raise(SIGINT);
    }
}

static void check_for_braces(int chr) {
    if (has_open_quotes()) {
        return;
    }

    if (chr != '{' && chr != '}') {
        return;
    }

    if (chr == '{') {
        ++open_braces;
    } else if (chr == '}') {
        --open_braces;
    }

    if (open_braces < 0) {
        fprintf(stderr, "-%s: unexpected '}'", SHELL_NAME);
        raise(SIGINT);
    }
}

static void check_for_conds(int chr) { (void) chr; }

static void check_for_loops(int chr) { (void) chr; }

static void check_for_pipes_and_OR_logic_ops(int chr) {
    if (has_open_quotes() || chr != '|') {
        if (chr != '\n' && chr != ' ') {
            unfinished_pipes = 0;
        }

        return;
    }

    ++unfinished_pipes;
}

static void check_for_AND_logic_ops(int chr) {
    if (has_open_quotes() || chr != '&') {
        if (chr != '\n' && chr != ' ') {
            unfinished_logic_and = 0;
        }

        return;
    }

    ++unfinished_logic_and;
}

static void check_for_backslash(int chr) {
    if (chr == '\\') {
        open_backslashes = !open_backslashes;
    } else if (chr != '\n') {
        open_backslashes = false;
    }

    unfinished_backslash = open_backslashes && chr == '\n';
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

    open_backslashes = false;
    unfinished_backslash = false;
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
