#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    FILE *file;
    char *fileName;
    int lines = 0;
    int words = 0;
    int chars = 0;
    int inWord = 0;
    int readFromStdin = 0;

    // 解析命令行参数
    if (argc < 2) {
        readFromStdin = 1;
        fileName = "stdin";
    } else {
        fileName = argv[1];
        file = fopen(fileName, "r");
        if (file == NULL) {
            printf("Error: Cannot open file '%s'\n", fileName);
            return 1;
        }
    }

    // 选择输入源
    FILE *input = readFromStdin ? stdin : file;
    char c;

    // 读取并统计
    while ((c = fgetc(input)) != EOF) {
        chars++;

        if (c == '\n') {
            lines++;
        }

        if (isspace(c)) {
            inWord = 0;
        } else if (!inWord) {
            words++;
            inWord = 1;
        }
    }

    // 如果文件没有以换行符结束，增加一行计数
    if (chars > 0 && c != '\n') {
        lines++;
    }

    // 关闭文件
    if (!readFromStdin) {
        fclose(file);
    }

    // 打印结果
    printf("%6d %6d %6d %s\n", lines, words, chars, fileName);

    return 0;
}