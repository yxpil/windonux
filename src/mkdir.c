#include <windows.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: mkdir directory [directory...]\n");
        return 1;
    }

    int successCount = 0;
    int errorCount = 0;

    for (int i = 1; i < argc; i++)
    {
        BOOL result;

        if (strcmp(argv[i], "-p") == 0)
        {
            if (i + 1 < argc)
            {
                i++;
                result = CreateDirectoryA(argv[i], NULL);
                if (!result && GetLastError() != ERROR_ALREADY_EXISTS)
                {
                    result = CreateDirectoryExA(NULL, argv[i], NULL);
                }
            }
        }
        else
        {
            result = CreateDirectoryA(argv[i], NULL);
        }

        if (result || GetLastError() == ERROR_ALREADY_EXISTS)
        {
            successCount++;
        }
        else
        {
            printf("Error: Cannot create directory '%s' (%d)\n", argv[i], GetLastError());
            errorCount++;
        }
    }

    return errorCount > 0 ? 1 : 0;
}