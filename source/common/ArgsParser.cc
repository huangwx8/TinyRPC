#include <common/ArgsParser.hh>

#include <string.h>
#include <stdlib.h>

/**
 * hints: trie tree is a more efficient way
 */

const char* parse_str(int argc, char* argv[], const char* flag)
{
    int len = strlen(flag);

    for (int i = 1; i < argc; i++)
    {
        if (strncmp(argv[i], flag, len) == 0)
        {
            return argv[i] + len;
        }
    }
    return nullptr;
}

int parse_int(int argc, char* argv[], const char* flag)
{
    int len = strlen(flag);

    for (int i = 1; i < argc; i++)
    {
        if (strncmp(argv[i], flag, len) == 0)
        {
            return atoi(argv[i] + len);
        }
    }
    return -1;
}
