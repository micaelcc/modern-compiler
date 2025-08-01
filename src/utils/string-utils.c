#include "string-utils.h"

char* read_file_to_string(const char *file_path) {
    FILE *file = fopen(file_path, "rb");
    if (!file) {
        perror("Failed to open file");
        return NULL;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        perror("fseek failed");
        fclose(file);
        return NULL;
    }

    long filesize = ftell(file);
    if (filesize < 0) {
        perror("ftell failed");
        fclose(file);
        return NULL;
    }

    rewind(file);

    char *buffer = (char*)malloc(filesize + 1);
    if (!buffer) {
        perror("malloc failed");
        fclose(file);
        return NULL;
    }

    size_t read_size = fread(buffer, 1, filesize, file);
    if (read_size != (size_t)filesize) {
        perror("fread failed");
        free(buffer);
        fclose(file);
        return NULL;
    }

    buffer[filesize] = '\0';

    fclose(file);
    return buffer;
}

void replace_char(char *s, char find, char replace)
{
    int i = 0;

    while (s[i])
    {
        if (s[i] == find)
        {
            if (s[i + 1] == find)
            {
                s[i + 1] = replace;
                i++;
            }
            else
            {
                s[i] = replace;
            }
        }
        i++;
    }
}

void split_string(const char *str, const char *delim, char result[MAX_SPLIT_LEN][MAX_SPLIT_LEN], int *count)
{
    char copy[strlen(str) + 1];
    strcpy(copy, str);

    *count = 0;
    char *token = strtok(copy, delim);

    while (token && *count < MAX_SPLIT_LEN)
    {
        strncpy(result[*count], token, MAX_SPLIT_LEN - 1);
        result[*count][MAX_SPLIT_LEN - 1] = '\0';
        (*count)++;
        token = strtok(NULL, delim);
    }
}

void remove_char(char *str, char target)
{
    char *src = str, *dst = str;

    while (*src)
    {
        if (*src != target)
        {
            *dst = *src;
            dst++;
        }
        src++;
    }
    *dst = '\0';
}

void remove_char_by_index(char *str, int target)
{
    int len = strlen(str);

    if (target < 0 || target >= len)
    {
        return;
    }

    for (int i = target; i < len; i++)
    {
        str[i] = str[i + 1];
    }
}

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
    value = (char *)malloc(SIZE+1 * sizeof(char));

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
