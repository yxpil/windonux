#include <windows.h>
#include <stdio.h>

int main() {
    char username[256];
    DWORD usernameLen = sizeof(username);

    if (GetUserName(username, &usernameLen)) {
        printf("%s\n", username);
        return 0;
    } else {
        printf("Error: Cannot get username (%d)\n", GetLastError());
        return 1;
    }
}