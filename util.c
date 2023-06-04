/***************************************************
 * Written by...: Ning Liu
 * Date Written.: May 27, 2023
 * Purpose......: Util
 * Assignments..: Resume Rater
 * Compilation..: gcc -o out main.c string_linked_list.c text_file_parser.c tire_tree.c util.c read_dir.c
 * How to run...: `./out.exe` or `./out`.
 ***************************************************/

#include <ctype.h>
#include <string.h>
#include "./util.h"

/**
 * Convert the string to lowercase.
 * @param str
 * @return
 */
char *my_strlwr(char *str) {
    unsigned char *p = (unsigned char *)str;

    while (*p) {
        *p = tolower((unsigned char)*p);
        p++;
    }

    return str;
}

/**
 * Trim a string
 * @param str
 * @return The result string
 */
char *trim(char *str) {
    char *end;
    // ltrim
    while (isspace(*str)) {
        str++;
    }

    if (*str == 0) {
        return str;
    }

    // rtrim
    end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) {
        end--;
    }

    // null terminator
    *(end + 1) = 0;

    return str;
}
