#include <windows.h>
#include <stdio.h>
#include <tchar.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: kill [-f] pid [pid...]\n");
        return 1;
    }

    BOOL force = FALSE;
    int startIndex = 1;

    if (strcmp(argv[1], "-f") == 0)
    {
        force = TRUE;
        startIndex = 2;
        if (argc < 3)
        {
            printf("Usage: kill [-f] pid [pid...]\n");
            return 1;
        }
    }

    int successCount = 0;
    int errorCount = 0;

    for (int i = startIndex; i < argc; i++)
    {
        DWORD pid = atoi(argv[i]);
        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);

        if (hProcess == NULL)
        {
            printf("Error: Cannot open process %d (%d)\n", pid, GetLastError());
            errorCount++;
            continue;
        }

        BOOL result = TerminateProcess(hProcess, 0);
        CloseHandle(hProcess);

        if (result)
        {
            successCount++;
        }
        else
        {
            printf("Error: Cannot terminate process %d (%d)\n", pid, GetLastError());
            errorCount++;
        }
    }

    return errorCount > 0 ? 1 : 0;
}