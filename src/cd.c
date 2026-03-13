#include <windows.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        // 默认切换到用户主目录
        char homeDir[MAX_PATH];
        if (SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, homeDir)) {
            printf("Error: Cannot get home directory\n");
            return 1;
        }
        if (SetCurrentDirectoryA(homeDir)) {
            printf("Changed directory to: %s\n", homeDir);
            return 0;
        }
    } else {
        // 切换到指定目录
        if (SetCurrentDirectoryA(argv[1])) {
            char currentDir[MAX_PATH];
            GetCurrentDirectoryA(sizeof(currentDir), currentDir);
            printf("Changed directory to: %s\n", currentDir);
            return 0;
        }
    }

    printf("Error: Cannot change directory to '%s' (%d)\n", 
           (argc < 2) ? "home" : argv[1], GetLastError());
    printf("Note: This program runs in its own process, so the parent shell's directory is not changed.\n");
    printf("To change the shell's directory, use Windows' native 'cd' command or create a batch file.\n");
    return 1;
}