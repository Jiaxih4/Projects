/**
 * vector
 * CS 341 - Fall 2023
 */
#include "sstring.h"

int main(int argc, char *argv[]) {
    // TODO create some tests
    sstring *str = cstr_to_sstring("this is ee a drill");
    printf("new string is: %s\n", sstring_to_cstr(str));
    char *news = sstring_slice(str, 2, 10);
    printf("new string is: %s\n", news);
    free(news);
    sstring_destroy(str);
    return 0;
}