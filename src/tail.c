#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
        // 对于标准输入，我们需要保存所有行
        char **lines = NULL;
        int lineCount = 0;
        int lineCapacity = 0;
        const int INITIAL_CAPACITY = 100;

        // 分配初始内存
        lines = (char **)malloc(INITIAL_CAPACITY * sizeof(char *));
        if (lines == NULL) {
            printf("Error: Out of memory\n");
            return 1;
        }
        lineCapacity = INITIAL_CAPACITY;

        // 读取所有行
        char line[1024];
        while (fgets(line, sizeof(line), stdin)) {
            // 检查是否需要扩展内存
            if (lineCount >= lineCapacity) {
                lineCapacity *= 2;
                lines = (char **)realloc(lines, lineCapacity * sizeof(char *));
                if (lines == NULL) {
                    printf("Error: Out of memory\n");
                    return 1;
                }
            }

            // 复制行到动态分配的内存
            lines[lineCount] = (char *)malloc((strlen(line) + 1) * sizeof(char));
            if (lines[lineCount] == NULL) {
                printf("Error: Out of memory\n");
                return 1;
            }
            strcpy(lines[lineCount], line);
            lineCount++;
        }

        // 显示最后几行
        int startLine = (lineCount > linesToShow) ? (lineCount - linesToShow) : 0;
        for (int i = startLine; i < lineCount; i++) {
            printf("%s", lines[i]);
            free(lines[i]);
        }
        free(lines);
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

            // 计算文件大小
            fseek(file, 0, SEEK_END);
            long fileSize = ftell(file);
            fseek(file, 0, SEEK_SET);

            // 对于小文件，可以直接读取所有行
            if (fileSize < 1024 * 1024) { // 小于1MB
                char **lines = NULL;
                int lineCount = 0;
                int lineCapacity = 0;
                const int INITIAL_CAPACITY = 100;

                // 分配初始内存
                lines = (char **)malloc(INITIAL_CAPACITY * sizeof(char *));
                if (lines == NULL) {
                    printf("Error: Out of memory\n");
                    fclose(file);
                    continue;
                }
                lineCapacity = INITIAL_CAPACITY;

                // 读取所有行
                char line[1024];
                while (fgets(line, sizeof(line), file)) {
                    // 检查是否需要扩展内存
                    if (lineCount >= lineCapacity) {
                        lineCapacity *= 2;
                        lines = (char **)realloc(lines, lineCapacity * sizeof(char *));
                        if (lines == NULL) {
                            printf("Error: Out of memory\n");
                            fclose(file);
                            continue;
                        }
                    }

                    // 复制行到动态分配的内存
                    lines[lineCount] = (char *)malloc((strlen(line) + 1) * sizeof(char));
                    if (lines[lineCount] == NULL) {
                        printf("Error: Out of memory\n");
                        fclose(file);
                        continue;
                    }
                    strcpy(lines[lineCount], line);
                    lineCount++;
                }

                // 显示最后几行
                int startLine = (lineCount > linesToShow) ? (lineCount - linesToShow) : 0;
                for (int j = startLine; j < lineCount; j++) {
                    printf("%s", lines[j]);
                    free(lines[j]);
                }
                free(lines);
            } else {
                // 对于大文件，使用更高效的方法
                long bufferSize = 1024 * 1024; // 1MB缓冲区
                char *buffer = (char *)malloc(bufferSize);
                if (buffer == NULL) {
                    printf("Error: Out of memory\n");
                    fclose(file);
                    continue;
                }

                // 从文件末尾开始读取
                long position = fileSize;
                int newlineCount = 0;
                char lastChar = '\0';

                while (position > 0 && newlineCount < linesToShow) {
                    long readSize = (position > bufferSize) ? bufferSize : position;
                    position -= readSize;
                    fseek(file, position, SEEK_SET);
                    fread(buffer, 1, readSize, file);

                    // 从缓冲区末尾开始查找换行符
                    for (long j = readSize - 1; j >= 0; j--) {
                        if (buffer[j] == '\n' && !(j == readSize - 1 && lastChar == '\n')) {
                            newlineCount++;
                            if (newlineCount == linesToShow) {
                                position += j + 1;
                                break;
                            }
                        }
                    }

                    lastChar = buffer[0];
                }

                // 显示最后几行
                fseek(file, position, SEEK_SET);
                char line[1024];
                while (fgets(line, sizeof(line), file)) {
                    printf("%s", line);
                }

                free(buffer);
            }

            fclose(file);
        }
    }

    return 0;
}