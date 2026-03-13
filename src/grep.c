#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: grep pattern file [file...]
");
        return 1;
    }

    char *pattern = argv[1];
    regex_t regex;
    int regexResult;

    // 编译正则表达式
    regexResult = regcomp(&regex, pattern, REG_EXTENDED | REG_ICASE);
    if (regexResult) {
        char errorMsg[100];
        regerror(regexResult, &regex, errorMsg, sizeof(errorMsg));
        printf("Error: Invalid regex pattern: %s\n", errorMsg);
        return 1;
    }

    int matchCount = 0;

    // 处理每个文件
    for (int i = 2; i < argc; i++) {
        FILE *file = fopen(argv[i], "r");
        if (file == NULL) {
            printf("Error: Cannot open file '%s'\n", argv[i]);
            continue;
        }

        char line[1024];
        int lineNum = 0;

        // 逐行读取文件
        while (fgets(line, sizeof(line), file)) {
            lineNum++;
            regmatch_t matches[1];

            // 搜索模式
            if (regexec(&regex, line, 1, matches, 0) == 0) {
                // 如果有多个文件，显示文件名
                if (argc > 3) {
                    printf("%s:", argv[i]);
                }
                printf("%d:%s", lineNum, line);
                matchCount++;
            }
        }

        fclose(file);
    }

    // 释放正则表达式
    regfree(&regex);

    return (matchCount > 0) ? 0 : 1;
}