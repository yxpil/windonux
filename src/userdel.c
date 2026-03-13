#include <windows.h>
#include <stdio.h>
#include <lm.h>

#pragma comment(lib, "netapi32.lib")

int main(int argc, char *argv[]) {
    NET_API_STATUS status;
    char username[256];
    BOOL removeHomeDir = FALSE;

    if (argc < 2) {
        printf("Usage: userdel [-r] username\n");
        return 1;
    }

    int argIndex = 1;
    if (strcmp(argv[argIndex], "-r") == 0) {
        removeHomeDir = TRUE;
        argIndex++;
    }

    if (argIndex >= argc) {
        printf("Usage: userdel [-r] username\n");
        return 1;
    }

    strncpy(username, argv[argIndex], sizeof(username) - 1);
    username[sizeof(username) - 1] = '\0';

    // 删除用户
    status = NetUserDel(NULL, username);

    if (status == NERR_Success) {
        printf("User '%s' deleted successfully\n", username);
    } else {
        switch (status) {
            case NERR_UserNotFound:
                printf("Error: User '%s' not found\n", username);
                break;
            case ERROR_ACCESS_DENIED:
                printf("Error: Access denied. You need administrator privileges.\n");
                break;
            default:
                printf("Error: NetUserDel failed with error: %d\n", status);
                break;
        }
        return 1;
    }

    if (removeHomeDir) {
        // 在Windows中，用户主目录通常在C:\Users\username
        char homeDir[MAX_PATH];
        snprintf(homeDir, sizeof(homeDir), "C:\\Users\\%s", username);
        
        // 删除主目录（需要递归删除）
        SHFILEOPSTRUCTA fileOp;
        memset(&fileOp, 0, sizeof(fileOp));
        fileOp.wFunc = FO_DELETE;
        fileOp.pFrom = homeDir;
        fileOp.fFlags = FOF_NOCONFIRMATION | FOF_SILENT | FOF_NOERRORUI | FOF_ALLOWUNDO;
        
        int result = SHFileOperationA(&fileOp);
        if (result != 0) {
            printf("Warning: Could not remove home directory '%s' (error %d)\n", homeDir, result);
        } else {
            printf("Home directory '%s' removed successfully\n", homeDir);
        }
    }

    return 0;
}