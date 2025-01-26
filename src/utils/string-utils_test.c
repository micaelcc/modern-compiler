#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "string-utils.h"

void test_substring()
{
    char *str = "abcdefgh";
    char *result = substring(str, 2, 5);

    assert(strcmp("cdef", result) == 0);

    free(result);
}

void test_contains()
{
    const char *arr[] = {"key", "id", "integer", NULL};

    bool result_true = contains(arr, "id");

    assert(result_true);

    bool result_false = contains(arr, "float");

    assert(!result_false);
}

int main()
{
    test_substring();
    test_contains();

    return 0;
}