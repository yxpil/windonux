#include <windows.h>
#include <stdio.h>
#include <tchar.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: mv source destination\n");
        return 1;
    }

    const char *src = argv[1];
    const char *dest = argv[2];
    BOOL result;

    DWORD srcAttr = GetFileAttributesA(src);
    DWORD destAttr = GetFileAttributesA(dest);

    if (srcAttr == INVALID_FILE_ATTRIBUTES) {
        printf("Error: Cannot access source '%s' (%d)\n", src, GetLastError());
        return 1;
    }

    if (destAttr != INVALID_FILE_ATTRIBUTES && (destAttr & FILE_ATTRIBUTE_DIRECTORY)) {
        // 目标是目录，构建完整的目标路径
        char fullDest[MAX_PATH];
        const char *fileName = strrchr(src, '\\');
        if (fileName == NULL) {
            fileName = src;
        } else {
            fileName++;
        }
        _snprintf(fullDest, sizeof(fullDest), "%s\\%s", dest, fileName);
        result = MoveFileExA(src, fullDest, MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING);
    } else {
        // 目标是文件或不存在，直接移动
        result = MoveFileExA(src, dest, MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING);
    }

    if (result) {
        return 0;
    } else {
        printf("Error: Move failed (%d)\n", GetLastError());
        return 1;
    }
}