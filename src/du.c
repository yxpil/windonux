#include <windows.h>
#include <stdio.h>
#include <tchar.h>

// 递归计算目录大小
LONGLONG calculateDirectorySize(LPCTSTR szDir) {
    TCHAR szPath[MAX_PATH];
    HANDLE hFind = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATA fdFile;
    LONGLONG totalSize = 0;

    // 构建搜索路径
    _stprintf(szPath, TEXT("%s\\*"), szDir);

    // 查找第一个文件
    hFind = FindFirstFile(szPath, &fdFile);
    if (hFind == INVALID_HANDLE_VALUE) {
        return 0;
    }

    do {
        // 跳过 . 和 ..
        if (_tcscmp(fdFile.cFileName, TEXT(".")) == 0 || _tcscmp(fdFile.cFileName, TEXT("..")) == 0) {
            continue;
        }

        // 构建完整路径
        _stprintf(szPath, TEXT("%s\\%s"), szDir, fdFile.cFileName);

        if (fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            // 如果是目录，递归计算大小
            totalSize += calculateDirectorySize(szPath);
        } else {
            // 如果是文件，添加文件大小
            totalSize += (LONGLONG)fdFile.nFileSizeLow | ((LONGLONG)fdFile.nFileSizeHigh << 32);
        }
    } while (FindNextFile(hFind, &fdFile));

    FindClose(hFind);

    return totalSize;
}

// 格式化大小为易读的格式
void formatSize(LONGLONG size, TCHAR* buffer, DWORD bufferSize) {
    const TCHAR* units[] = { TEXT("B"), TEXT("KB"), TEXT("MB"), TEXT("GB"), TEXT("TB") };
    int unitIndex = 0;
    double formattedSize = (double)size;

    while (formattedSize >= 1024 && unitIndex < 4) {
        formattedSize /= 1024;
        unitIndex++;
    }

    _stprintf(buffer, TEXT("%.2f %s"), formattedSize, units[unitIndex]);
}

int main(int argc, char *argv[]) {
    const TCHAR* directory = TEXT(".");
    BOOL showSummaryOnly = FALSE;

    // 解析命令行参数
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0) {
            showSummaryOnly = TRUE;
        } else {
            directory = argv[i];
        }
    }

    LONGLONG totalSize = calculateDirectorySize(directory);
    TCHAR formattedSize[20];
    formatSize(totalSize, formattedSize, sizeof(formattedSize));

    if (showSummaryOnly) {
        _tprintf(TEXT("%s\t%s\n"), formattedSize, directory);
    } else {
        // 如果不是只显示摘要，遍历目录内容
        TCHAR szPath[MAX_PATH];
        HANDLE hFind = INVALID_HANDLE_VALUE;
        WIN32_FIND_DATA fdFile;

        _stprintf(szPath, TEXT("%s\\*"), directory);
        hFind = FindFirstFile(szPath, &fdFile);

        if (hFind != INVALID_HANDLE_VALUE) {
            do {
                if (_tcscmp(fdFile.cFileName, TEXT(".")) == 0 || _tcscmp(fdFile.cFileName, TEXT("..")) == 0) {
                    continue;
                }

                _stprintf(szPath, TEXT("%s\\%s"), directory, fdFile.cFileName);
                LONGLONG itemSize;

                if (fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    itemSize = calculateDirectorySize(szPath);
                } else {
                    itemSize = (LONGLONG)fdFile.nFileSizeLow | ((LONGLONG)fdFile.nFileSizeHigh << 32);
                }

                TCHAR itemFormattedSize[20];
                formatSize(itemSize, itemFormattedSize, sizeof(itemFormattedSize));
                _tprintf(TEXT("%s\t%s\\%s\n"), itemFormattedSize, directory, fdFile.cFileName);
            } while (FindNextFile(hFind, &fdFile));

            FindClose(hFind);
        }

        _tprintf(TEXT("%s\t%s\t(total)\n"), formattedSize, directory);
    }

    return 0;
}