#include <windows.h>
#include <stdio.h>
#include <tchar.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: which command [command...]
");
        return 1;
    }

    int successCount = 0;
    int errorCount = 0;

    // 获取系统PATH环境变量
    TCHAR envPath[MAX_PATH * 10];
    if (!GetEnvironmentVariable(TEXT("PATH"), envPath, sizeof(envPath))) {
        printf("Error: Cannot get PATH environment variable\n");
        return 1;
    }

    // 处理每个命令参数
    for (int i = 1; i < argc; i++) {
        TCHAR commandName[MAX_PATH];
        TCHAR fullPath[MAX_PATH];
        BOOL found = FALSE;

        // 添加.exe扩展名（如果没有的话）
        if (_tcsrchr(argv[i], TEXT('.')) == NULL) {
            _stprintf(commandName, TEXT("%s.exe"), argv[i]);
        } else {
            _tcscpy(commandName, argv[i]);
        }

        // 复制PATH环境变量以便修改
        TCHAR pathCopy[MAX_PATH * 10];
        _tcscpy(pathCopy, envPath);

        // 分割PATH环境变量
        TCHAR* token = _tcstok(pathCopy, TEXT(";"));
        while (token != NULL) {
            // 构建完整路径
            _stprintf(fullPath, TEXT("%s\\%s"), token, commandName);

            // 检查文件是否存在
            if (GetFileAttributes(fullPath) != INVALID_FILE_ATTRIBUTES) {
                _tprintf(TEXT("%s\n"), fullPath);
                found = TRUE;
                successCount++;
                break;
            }

            token = _tcstok(NULL, TEXT(";"));
        }

        if (!found) {
            _tprintf(TEXT("which: %s: command not found\n"), argv[i]);
            errorCount++;
        }
    }

    return (errorCount > 0) ? 1 : 0;
}