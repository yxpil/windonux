#include <windows.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: cat file [file...]
");
        return 1;
    }

    int successCount = 0;
    int errorCount = 0;

    // 处理每个文件参数
    for (int i = 1; i < argc; i++) {
        FILE *file = fopen(argv[i], "r");
        if (file == NULL) {
            printf("Error: Cannot open file '%s' (%d)\n", argv[i], GetLastError());
            errorCount++;
            continue;
        }

        // 读取并显示文件内容
        char buffer[1024];
        size_t bytesRead;
        while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
            fwrite(buffer, 1, bytesRead, stdout);
        }

        fclose(file);
        successCount++;

        // 如果有多个文件，在文件之间添加空行
        if (i < argc - 1) {
            printf("\n\n");
        }
    }

    if (errorCount > 0) {
        return 1;
    }

    return 0;
}