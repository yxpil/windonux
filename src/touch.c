#include <windows.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: touch file [file...]\n");
        return 1;
    }

    int successCount = 0;
    int errorCount = 0;

    for (int i = 1; i < argc; i++)
    {
        HANDLE hFile;
        BOOL result;

        hFile = CreateFileA(argv[i], GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

        if (hFile == INVALID_HANDLE_VALUE)
        {
            printf("Error: Cannot open/create file '%s' (%d)\n", argv[i], GetLastError());
            errorCount++;
            continue;
        }

        SYSTEMTIME st;
        GetSystemTime(&st);
        FILETIME ft;
        SystemTimeToFileTime(&st, &ft);

        result = SetFileTime(hFile, &ft, &ft, &ft);

        if (!result)
        {
            printf("Error: Cannot set file time for '%s' (%d)\n", argv[i], GetLastError());
            errorCount++;
        }
        else
        {
            successCount++;
        }

        CloseHandle(hFile);
    }

    return errorCount > 0 ? 1 : 0;
}