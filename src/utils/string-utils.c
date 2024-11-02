#include "string-utils.h"

char *input_string()
{
    char *str = NULL;
    size_t size = 0;
    ssize_t len;

    len = getline(&str, &size, stdin);

    if (len == -1)
    {
        perror("Getline error!");
        exit(1);
    }

    str[strcspn(str, "\n")] = '\0';

    return str;
}

char *substring(const char *s, int start, int end)
{
    int SIZE;
    char *value;

    SIZE = end - start + 1;
    value = malloc(SIZE * sizeof(char));

    strncpy(value, s + start, SIZE);
    value[SIZE] = '\0';

    return value;
}

bool contains(const char *arr[], const char s[])
{
    for (size_t i = 0; arr[i]; i++)
        if (0 == strcmp(s, arr[i]))
            return true;

    return false;
}
