#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 比较函数，用于qsort
int compareLines(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

int main(int argc, char *argv[]) {
    FILE *file;
    char **lines = NULL;
    int lineCount = 0;
    int lineCapacity = 0;
    const int INITIAL_CAPACITY = 100;

    // 确定输入源
    if (argc == 1) {
        file = stdin;
    } else {
        file = fopen(argv[1], "r");
        if (file == NULL) {
            printf("Error: Cannot open file '%s'\n", argv[1]);
            return 1;
        }
    }

    // 分配初始内存
    lines = (char **)malloc(INITIAL_CAPACITY * sizeof(char *));
    if (lines == NULL) {
        printf("Error: Out of memory\n");
        return 1;
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

    // 排序行
    qsort(lines, lineCount, sizeof(char *), compareLines);

    // 输出排序后的行
    for (int i = 0; i < lineCount; i++) {
        printf("%s", lines[i]);
        free(lines[i]);
    }

    // 释放内存
    free(lines);
    if (file != stdin) {
        fclose(file);
    }

    return 0;
}