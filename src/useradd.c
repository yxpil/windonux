#include <windows.h>
#include <stdio.h>
#include <lm.h>

#pragma comment(lib, "netapi32.lib")

int main(int argc, char *argv[]) {
    NET_API_STATUS status;
    USER_INFO_1 ui;
    char username[256];
    char password[256];

    if (argc < 2) {
        printf("Usage: useradd username\n");
        return 1;
    }

    strncpy(username, argv[1], sizeof(username) - 1);
    username[sizeof(username) - 1] = '\0';

    // 提示输入密码
    printf("Enter password for new user: ");
    int i = 0;
    char ch;
    while (i < sizeof(password) - 1) {
        ch = _getch();
        if (ch == 13) break; // Enter键
        if (ch == 8) { // Backspace键
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';
    printf("\n");

    // 准备用户信息
    ui.usri1_name = username;
    ui.usri1_password = password;
    ui.usri1_priv = USER_PRIV_USER;
    ui.usri1_home_dir = NULL;
    ui.usri1_comment = "User added by Linux-like useradd command";
    ui.usri1_flags = UF_SCRIPT;
    ui.usri1_script_path = NULL;

    // 创建用户
    status = NetUserAdd(NULL, 1, (LPBYTE)&ui, NULL);

    if (status == NERR_Success) {
        printf("User '%s' created successfully\n", username);
    } else {
        switch (status) {
            case NERR_DuplicateName:
                printf("Error: User '%s' already exists\n", username);
                break;
            case NERR_PasswordTooShort:
                printf("Error: Password is too short\n");
                break;
            case NERR_InvalidPassword:
                printf("Error: Invalid password\n");
                break;
            default:
                printf("Error: NetUserAdd failed with error: %d\n", status);
                break;
        }
        return 1;
    }

    return 0;
}