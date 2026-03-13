#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <string.h>

// 递归搜索文件
void findFiles(LPCTSTR szDir, LPCTSTR szPattern) {
    TCHAR szPath[MAX_PATH];
    HANDLE hFind = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATA fdFile;

    // 构建搜索路径
    _stprintf(szPath, TEXT("%s\\*"), szDir);

    // 查找第一个文件
    hFind = FindFirstFile(szPath, &fdFile);
    if (hFind == INVALID_HANDLE_VALUE) {
        return;
    }

    do {
        // 跳过 . 和 ..
        if (_tcscmp(fdFile.cFileName, TEXT(".")) == 0 || _tcscmp(fdFile.cFileName, TEXT("..")) == 0) {
            continue;
        }

        // 构建完整路径
        _stprintf(szPath, TEXT("%s\\%s"), szDir, fdFile.cFileName);

        if (fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            // 如果是目录，递归搜索
            findFiles(szPath, szPattern);
        } else {
            // 如果是文件，检查是否匹配模式
            if (szPattern == NULL || szPattern[0] == 0 || _tcsstr(fdFile.cFileName, szPattern) != NULL) {
                _tprintf(TEXT("%s\n"), szPath);
            }
        }
    } while (FindNextFile(hFind, &fdFile));

    FindClose(hFind);
}

int main(int argc, char *argv[]) {
    LPCTSTR directory = TEXT(".");
    LPCTSTR pattern = NULL;

    // 解析命令行参数
    if (argc == 2) {
        // find pattern
        pattern = argv[1];
    } else if (argc == 3) {
        // find directory pattern
        directory = argv[1];
        pattern = argv[2];
    } else {
        // find .
        pattern = NULL;
    }

    findFiles(directory, pattern);
    return 0;
}