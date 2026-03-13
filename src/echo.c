#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int i = 1;
    int newline = 1; // 默认添加换行符

    // 检查是否有-n参数（不添加换行符）
    if (argc > 1 && strcmp(argv[1], "-n") == 0) {
        newline = 0;
        i = 2;
    }

    // 打印所有参数
    while (i < argc) {
        printf("%s", argv[i]);
        if (i < argc - 1) {
            printf(" ");
        }
        i++;
    }

    // 如果需要，添加换行符
    if (newline) {
        printf("\n");
    }

    return 0;
}