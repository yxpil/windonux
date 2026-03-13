#include <windows.h>
#include <stdio.h>

// 将Linux权限模式转换为Windows文件属性
DWORD linuxModeToWindowsAttr(int mode) {
    DWORD attr = 0;

    // 获取当前文件属性
    // 注意：这里简化处理，主要关注只读属性
    if ((mode & 0222) == 0) { // 没有写入权限
        attr |= FILE_ATTRIBUTE_READONLY;
    }

    return attr;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: chmod mode file [file...]
");
        return 1;
    }

    // 解析权限模式
    int mode = 0;
    if (sscanf(argv[1], "%o", &mode) != 1) {
        printf("Error: Invalid mode format\n");
        printf("Usage: chmod mode file [file...]
");
        return 1;
    }

    int successCount = 0;
    int errorCount = 0;

    // 处理每个文件参数
    for (int i = 2; i < argc; i++) {
        DWORD currentAttr = GetFileAttributesA(argv[i]);
        if (currentAttr == INVALID_FILE_ATTRIBUTES) {
            printf("Error: Cannot access file '%s' (%d)\n", argv[i], GetLastError());
            errorCount++;
            continue;
        }

        // 清除现有属性（保留目录等重要属性）
        DWORD newAttr = currentAttr & (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_ARCHIVE);

        // 添加新的权限属性
        newAttr |= linuxModeToWindowsAttr(mode);

        // 设置新属性
        if (SetFileAttributesA(argv[i], newAttr)) {
            successCount++;
        } else {
            printf("Error: Cannot set attributes for '%s' (%d)\n", argv[i], GetLastError());
            errorCount++;
        }
    }

    if (errorCount > 0) {
        return 1;
    }

    return 0;
}