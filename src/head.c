#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *file;
    int linesToShow = 10;
    int fileStartIndex = 1;

    // 解析命令行参数
    if (argc > 1 && argv[1][0] == '-') {
        linesToShow = atoi(&argv[1][1]);
        if (linesToShow <= 0) {
            linesToShow = 10;
        }
        fileStartIndex = 2;
    }

    // 如果没有指定文件，从标准输入读取
    if (fileStartIndex >= argc) {
        file = stdin;
        int lineCount = 0;
        char line[1024];
        while (lineCount < linesToShow && fgets(line, sizeof(line), file)) {
            printf("%s", line);
            lineCount++;
        }
    } else {
        // 处理每个文件
        for (int i = fileStartIndex; i < argc; i++) {
            file = fopen(argv[i], "r");
            if (file == NULL) {
                printf("Error: Cannot open file '%s'\n", argv[i]);
                continue;
            }

            // 如果有多个文件，显示文件名
            if (argc > fileStartIndex + 1) {
                printf("==> %s <==\n", argv[i]);
            }

            // 显示文件的前几行
            int lineCount = 0;
            char line[1024];
            while (lineCount < linesToShow && fgets(line, sizeof(line), file)) {
                printf("%s", line);
                lineCount++;
            }

            fclose(file);
        }
    }

    return 0;
}