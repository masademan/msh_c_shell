#include "shell_line_check.h"

// Static internal vars
static int prev_c = '\0';

static bool open_single_quotes = false;
static bool open_double_quotes = false;

static bool open_parens = false;
static bool open_braces = false;

static bool open_conds = false;
static bool open_loops = false;

static bool unfinished_pipes = false;
static bool unfinished_logic_ops = false;


// Funcs that check the static internal vars

void check_next_char(int chr) { (void) chr; }

bool is_complete_command(void) { return true; }

bool has_open_quotes(void) { return false; }

bool has_open_parens(void) { return false; }

bool has_open_braces(void) { return false; }

bool has_open_conditional(void) { return false; }

bool has_open_loop(void) { return false; }

bool has_unfinished_pipe(void) { return false; }

bool has_unfinished_logical_operator(void) { return false; }
