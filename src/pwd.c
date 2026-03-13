#include <windows.h>
#include <stdio.h>

int main() {
    char currentDir[MAX_PATH];

    if (GetCurrentDirectoryA(sizeof(currentDir), currentDir)) {
        printf("%s\n", currentDir);
        return 0;
    } else {
        printf("Error: GetCurrentDirectory failed (%d)\n", GetLastError());
        return 1;
    }
}