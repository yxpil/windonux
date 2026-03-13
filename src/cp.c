#include <windows.h>
#include <stdio.h>
#include <tchar.h>

// 复制单个文件
BOOL copyFile(LPCTSTR src, LPCTSTR dest) {
    return CopyFile(src, dest, FALSE);
}

// 递归复制目录
BOOL copyDirectoryRecursive(LPCTSTR srcDir, LPCTSTR destDir) {
    TCHAR srcPath[MAX_PATH];
    TCHAR destPath[MAX_PATH];
    HANDLE hFind = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATA fdFile;

    // 创建目标目录
    if (!CreateDirectory(destDir, NULL) && GetLastError() != ERROR_ALREADY_EXISTS) {
        return FALSE;
    }

    // 构建搜索路径
    _stprintf(srcPath, TEXT("%s\\*"), srcDir);

    // 查找第一个文件
    hFind = FindFirstFile(srcPath, &fdFile);
    if (hFind == INVALID_HANDLE_VALUE) {
        return FALSE;
    }

    do {
        // 跳过 . 和 ..
        if (_tcscmp(fdFile.cFileName, TEXT(".")) == 0 || _tcscmp(fdFile.cFileName, TEXT("..")) == 0) {
            continue;
        }

        // 构建源和目标完整路径
        _stprintf(srcPath, TEXT("%s\\%s"), srcDir, fdFile.cFileName);
        _stprintf(destPath, TEXT("%s\\%s"), destDir, fdFile.cFileName);

        if (fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            // 如果是目录，递归复制
            if (!copyDirectoryRecursive(srcPath, destPath)) {
                FindClose(hFind);
                return FALSE;
            }
        } else {
            // 如果是文件，直接复制
            if (!copyFile(srcPath, destPath)) {
                FindClose(hFind);
                return FALSE;
            }
        }
    } while (FindNextFile(hFind, &fdFile));

    FindClose(hFind);
    return TRUE;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: cp [-r] source destination\n");
        return 1;
    }

    BOOL recursive = FALSE;
    int srcIndex = 1;
    int destIndex = 2;

    // 检查是否有-r参数（递归复制）
    if (strcmp(argv[1], "-r") == 0) {
        recursive = TRUE;
        srcIndex = 2;
        destIndex = 3;
        if (argc < 4) {
            printf("Usage: cp -r source destination\n");
            return 1;
        }
    }

    const char *src = argv[srcIndex];
    const char *dest = argv[destIndex];
    BOOL result;

    DWORD srcAttr = GetFileAttributesA(src);
    DWORD destAttr = GetFileAttributesA(dest);

    if (srcAttr == INVALID_FILE_ATTRIBUTES) {
        printf("Error: Cannot access source '%s' (%d)\n", src, GetLastError());
        return 1;
    }

    if (srcAttr & FILE_ATTRIBUTE_DIRECTORY) {
        // 源是目录
        if (!recursive) {
            printf("Error: Source '%s' is a directory. Use -r to copy recursively.\n", src);
            return 1;
        }

        // 检查目标是否存在且是目录
        if (destAttr != INVALID_FILE_ATTRIBUTES && !(destAttr & FILE_ATTRIBUTE_DIRECTORY)) {
            printf("Error: Destination '%s' is not a directory.\n", dest);
            return 1;
        }

        // 递归复制目录
        result = copyDirectoryRecursive(src, dest);
    } else {
        // 源是文件
        if (destAttr != INVALID_FILE_ATTRIBUTES && (destAttr & FILE_ATTRIBUTE_DIRECTORY)) {
            // 目标是目录，构建完整的目标文件路径
            char fullDest[MAX_PATH];
            _snprintf(fullDest, sizeof(fullDest), "%s\\%s", dest, strrchr(src, '\\') ? strrchr(src, '\\') + 1 : src);
            result = copyFile(src, fullDest);
        } else {
            // 目标是文件，直接复制
            result = copyFile(src, dest);
        }
    }

    if (result) {
        return 0;
    } else {
        printf("Error: Copy failed (%d)\n", GetLastError());
        return 1;
    }
}