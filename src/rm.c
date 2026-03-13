#include <windows.h>
#include <stdio.h>
#include <tchar.h>

BOOL removeDirectoryRecursive(LPCTSTR szDir)
{
    TCHAR szPath[MAX_PATH];
    HANDLE hFind = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATA fdFile;

    _stprintf(szPath, TEXT("%s\\*"), szDir);

    hFind = FindFirstFile(szPath, &fdFile);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }

    do
    {
        if (_tcscmp(fdFile.cFileName, TEXT(".")) == 0 || _tcscmp(fdFile.cFileName, TEXT("..")) == 0)
        {
            continue;
        }

        _stprintf(szPath, TEXT("%s\\%s"), szDir, fdFile.cFileName);

        if (fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (!removeDirectoryRecursive(szPath))
            {
                FindClose(hFind);
                return FALSE;
            }
        }
        else
        {
            if (!DeleteFile(szPath))
            {
                FindClose(hFind);
                return FALSE;
            }
        }
    } while (FindNextFile(hFind, &fdFile));

    FindClose(hFind);

    return RemoveDirectory(szDir);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: rm [-r] file [file...]\n");
        return 1;
    }

    BOOL recursive = FALSE;
    int startIndex = 1;

    if (strcmp(argv[1], "-r") == 0 || strcmp(argv[1], "-rf") == 0)
    {
        recursive = TRUE;
        startIndex = 2;
        if (argc < 3)
        {
            printf("Usage: rm -r file [file...]\n");
            return 1;
        }
    }

    int successCount = 0;
    int errorCount = 0;

    for (int i = startIndex; i < argc; i++)
    {
        BOOL result;
        DWORD attributes = GetFileAttributesA(argv[i]);

        if (attributes == INVALID_FILE_ATTRIBUTES)
        {
            printf("Error: Cannot access '%s' (%d)\n", argv[i], GetLastError());
            errorCount++;
            continue;
        }

        if (attributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (recursive)
            {
                result = removeDirectoryRecursive(argv[i]);
            }
            else
            {
                printf("Error: '%s' is a directory. Use -r to remove recursively.\n", argv[i]);
                errorCount++;
                continue;
            }
        }
        else
        {
            result = DeleteFileA(argv[i]);
        }

        if (result)
        {
            successCount++;
        }
        else
        {
            printf("Error: Cannot remove '%s' (%d)\n", argv[i], GetLastError());
            errorCount++;
        }
    }

    return errorCount > 0 ? 1 : 0;
}