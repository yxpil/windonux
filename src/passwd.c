#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <lm.h>

#pragma comment(lib, "netapi32.lib")

// 隐藏输入密码的函数
void getPassword(char* password, int maxLen) {
    int i = 0;
    char ch;

    while (i < maxLen - 1) {
        ch = _getch();
        if (ch == 13) { // Enter键
            break;
        } else if (ch == 8) { // Backspace键
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
}

int main() {
    char username[256];
    char oldPassword[256];
    char newPassword[256];
    NET_API_STATUS status;

    // 获取当前用户名
    DWORD usernameLen = sizeof(username);
    if (!GetUserName(username, &usernameLen)) {
        printf("Error: GetUserName failed (%d)\n", GetLastError());
        return 1;
    }

    printf("Changing password for %s.\n", username);

    // 获取旧密码
    printf("Current password: ");
    getPassword(oldPassword, sizeof(oldPassword));

    // 获取新密码
    printf("New password: ");
    getPassword(newPassword, sizeof(newPassword));

    // 再次输入新密码确认
    char confirmPassword[256];
    printf("Retype new password: ");
    getPassword(confirmPassword, sizeof(confirmPassword));

    // 验证两次输入的新密码是否一致
    if (strcmp(newPassword, confirmPassword) != 0) {
        printf("Error: New passwords do not match\n");
        return 1;
    }

    // 修改密码
    status = NetUserChangePassword(NULL, username, oldPassword, newPassword);

    if (status == NERR_Success) {
        printf("Password changed successfully\n");
        return 0;
    } else {
        switch (status) {
            case NERR_InvalidPassword:
                printf("Error: Current password is incorrect\n");
                break;
            case NERR_PasswordTooShort:
                printf("Error: New password is too short\n");
                break;
            case NERR_PasswordTooRecent:
                printf("Error: New password was used recently\n");
                break;
            case NERR_PasswordHistConflict:
                printf("Error: New password conflicts with history\n");
                break;
            default:
                printf("Error: Password change failed (%d)\n", status);
                break;
        }
        return 1;
    }
}