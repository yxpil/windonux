#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>

#define MAX_CMD_LEN 1024
#define MAX_ARGS 32
#define HISTORY_SIZE 100

char *history[HISTORY_SIZE];
int history_count = 0;
int history_index = 0;

// 初始化历史记录
void init_history() {
    for (int i = 0; i < HISTORY_SIZE; i++) {
        history[i] = NULL;
    }
}

// 添加命令到历史记录
void add_to_history(char *cmd) {
    if (cmd == NULL || strlen(cmd) == 0) {
        return;
    }

    // 如果历史记录已满，删除最旧的记录
    if (history_count >= HISTORY_SIZE) {
        free(history[0]);
        for (int i = 1; i < HISTORY_SIZE; i++) {
            history[i - 1] = history[i];
        }
        history_count--;
    }

    // 添加新命令
    history[history_count] = strdup(cmd);
    history_count++;
    history_index = history_count;
}

// 从历史记录中获取命令
char *get_history(int index) {
    if (index < 0 || index >= history_count) {
        return NULL;
    }
    return history[index];
}

// 显示提示信息
void show_prompt() {
    char cwd[MAX_PATH];
    if (GetCurrentDirectoryA(MAX_PATH, cwd) != 0) {
        // 将Windows路径转换为类似Unix的路径
        for (int i = 0; cwd[i] != '\0'; i++) {
            if (cwd[i] == '\\') {
                cwd[i] = '/';
            }
        }
        printf("bash@windows:%s$ ", cwd);
    } else {
        printf("bash@windows:$ ");
    }
    fflush(stdout);
}

// 分割命令行参数
int parse_command(char *cmd, char **args) {
    int i = 0;
    char *token = strtok(cmd, " ");

    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    return i;
}

// 执行命令
int execute_command(char **args) {
    if (args[0] == NULL) {
        return 0;
    }

    // 内置命令
    if (strcmp(args[0], "exit") == 0) {
        return 1;
    } else if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            // 切换到用户主目录
            char home_dir[MAX_PATH];
            if (SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, home_dir) == S_OK) {
                SetCurrentDirectoryA(home_dir);
            }
        } else {
            if (SetCurrentDirectoryA(args[1]) != 0) {
                // 成功切换目录
            } else {
                printf("cd: %s: No such file or directory\n", args[1]);
            }
        }
        return 0;
    } else if (strcmp(args[0], "history") == 0) {
        for (int i = 0; i < history_count; i++) {
            printf("%3d %s\n", i + 1, history[i]);
        }
        return 0;
    }

    // 创建进程执行外部命令
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    char cmd_line[MAX_CMD_LEN];

    // 构建命令行
    cmd_line[0] = '\0';
    for (int i = 0; args[i] != NULL; i++) {
        strcat(cmd_line, args[i]);
        if (args[i + 1] != NULL) {
            strcat(cmd_line, " ");
        }
    }

    // 初始化结构
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // 创建进程
    if (!CreateProcessA(NULL, cmd_line, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        printf("%s: command not found\n", args[0]);
        return 0;
    }

    // 等待进程完成
    WaitForSingleObject(pi.hProcess, INFINITE);

    // 关闭句柄
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}

// 读取带历史记录的命令
char *read_command() {
    static char cmd[MAX_CMD_LEN];
    int pos = 0;
    char ch;
    int history_nav = 0;

    cmd[0] = '\0';

    while (1) {
        ch = _getch();

        if (ch == 13) { // Enter键
            cmd[pos] = '\0';
            printf("\n");
            return cmd;
        } else if (ch == 8) { // Backspace键
            if (pos > 0) {
                pos--;
                printf("\b \b");
            }
        } else if (ch == 27) { // ESC键
            // 处理方向键
            ch = _getch();
            if (ch == 91) {
                ch = _getch();
                if (ch == 65) { // 上箭头
                    if (history_index > 0) {
                        history_index--;
                        // 清除当前行
                        for (int i = 0; i < pos; i++) {
                            printf("\b \b");
                        }
                        // 显示历史命令
                        char *hist_cmd = get_history(history_index);
                        if (hist_cmd != NULL) {
                            strncpy(cmd, hist_cmd, MAX_CMD_LEN - 1);
                            cmd[MAX_CMD_LEN - 1] = '\0';
                            pos = strlen(cmd);
                            printf("%s", cmd);
                        }
                    }
                } else if (ch == 66) { // 下箭头
                    if (history_index < history_count - 1) {
                        history_index++;
                        // 清除当前行
                        for (int i = 0; i < pos; i++) {
                            printf("\b \b");
                        }
                        // 显示历史命令
                        char *hist_cmd = get_history(history_index);
                        if (hist_cmd != NULL) {
                            strncpy(cmd, hist_cmd, MAX_CMD_LEN - 1);
                            cmd[MAX_CMD_LEN - 1] = '\0';
                            pos = strlen(cmd);
                            printf("%s", cmd);
                        }
                    } else if (history_index == history_count - 1) {
                        history_index++;
                        // 清除当前行
                        for (int i = 0; i < pos; i++) {
                            printf("\b \b");
                        }
                        cmd[0] = '\0';
                        pos = 0;
                    }
                }
            }
        } else if (ch >= 32 && ch <= 126 && pos < MAX_CMD_LEN - 1) { // 可打印字符
            cmd[pos++] = ch;
            printf("%c", ch);
        }
    }
}

int main() {
    char *cmd;
    char *args[MAX_ARGS];
    int should_exit = 0;

    // 初始化
    init_history();

    // 显示欢迎信息
    printf("Linux-like Bash for Windows\n");
    printf("Type 'exit' to quit\n\n");

    // 主循环
    while (!should_exit) {
        show_prompt();
        cmd = read_command();

        // 添加到历史记录
        add_to_history(cmd);

        // 解析并执行命令
        parse_command(cmd, args);
        should_exit = execute_command(args);
    }

    // 清理历史记录
    for (int i = 0; i < HISTORY_SIZE; i++) {
        if (history[i] != NULL) {
            free(history[i]);
        }
    }

    return 0;
}