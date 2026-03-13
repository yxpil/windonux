#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <time.h>

// 获取文件属性的字符串表示
void printFileAttributes(DWORD attributes) {
    printf("%c", (attributes & FILE_ATTRIBUTE_DIRECTORY) ? 'd' : '-');
    printf("%c", (attributes & FILE_ATTRIBUTE_READONLY) ? 'r' : '-');
    printf("%c", (attributes & FILE_ATTRIBUTE_ARCHIVE) ? 'a' : '-');
    printf("%c", (attributes & FILE_ATTRIBUTE_HIDDEN) ? 'h' : '-');
    printf("%c", (attributes & FILE_ATTRIBUTE_SYSTEM) ? 's' : '-');
    printf(" ");
}

// 打印文件时间
void printFileTime(FILETIME *fileTime) {
    SYSTEMTIME st;
    FileTimeToSystemTime(fileTime, &st);
    printf("%04d-%02d-%02d %02d:%02d ", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute);
}

int main(int argc, char *argv[]) {
    WIN32_FIND_DATAA findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    char directory[MAX_PATH];
    char searchPath[MAX_PATH];

    // 确定要列出的目录
    if (argc > 1) {
        strncpy(directory, argv[1], sizeof(directory) - 1);
        directory[sizeof(directory) - 1] = '\0';
    } else {
        if (!GetCurrentDirectoryA(sizeof(directory), directory)) {
            printf("Error: GetCurrentDirectory failed (%d)\n", GetLastError());
            return 1;
        }
    }

    // 构建搜索路径
    snprintf(searchPath, sizeof(searchPath), "%s\\*", directory);

    // 查找第一个文件
    hFind = FindFirstFileA(searchPath, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Error: FindFirstFile failed (%d)\n", GetLastError());
        return 1;
    }

    // 遍历所有文件
    do {
        // 跳过 . 和 ..
        if (strcmp(findFileData.cFileName, ".") == 0 || strcmp(findFileData.cFileName, "..") == 0) {
            continue;
        }

        // 打印文件属性
        printFileAttributes(findFileData.dwFileAttributes);

        // 打印文件大小
        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            printf("%10lld ", (long long)findFileData.nFileSizeLow | ((long long)findFileData.nFileSizeHigh << 32));
        } else {
            printf("%10s ", "<DIR>");
        }

        // 打印最后修改时间
        printFileTime(&findFileData.ftLastWriteTime);

        // 打印文件名
        printf("%s\n", findFileData.cFileName);

    } while (FindNextFileA(hFind, &findFileData) != 0);

    if (GetLastError() != ERROR_NO_MORE_FILES) {
        printf("Error: FindNextFile failed (%d)\n", GetLastError());
        FindClose(hFind);
        return 1;
    }

    FindClose(hFind);
    return 0;
}