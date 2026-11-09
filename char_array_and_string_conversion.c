#include <stdio.h>
#include <stdlib.h>

#include "char_array_and_string_conversion.h"
#include "dynamic_array_char.h"
#include "safe_string.h"

String *da_join_to_string(const CharArray *da) {
    String *s = ss_new_length(da_count_char(da));

    for (size_t i = 0; i < da_count_char(da); i++) {
        char c;

        if (!da_get_char(da, i, &c)) {
            perror("da_get error");
            exit(EXIT_FAILURE);
        }

        us_set_at(s, i, c);
    }

    return s;
}

CharArray *ss_split_to_char_array(const String *ss) {
    CharArray *da = da_new_char();

    da_reserve_char(da, ss_length(ss));

    for (size_t i = 0; i < ss_length(ss); i++) {
        char c;

        if (!ss_at(ss, i, &c)) {
            perror("ss_at error");
            exit(EXIT_FAILURE);
        }

        da_add_char(da, c);
    }

    return da;
}
