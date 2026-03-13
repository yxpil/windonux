#include <windows.h>
#include <stdio.h>
#include <tchar.h>

int main() {
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;

    // 获取当前进程的令牌
    if (!OpenProcessToken(GetCurrentProcess(),
                         TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
                         &hToken)) {
        printf("Error: OpenProcessToken failed (%d)\n", GetLastError());
        return 1;
    }

    // 获取关机特权的LUID
    LookupPrivilegeValue(NULL,
                        SE_SHUTDOWN_NAME,
                        &tkp.Privileges[0].Luid);

    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    // 启用关机特权
    AdjustTokenPrivileges(hToken,
                         FALSE,
                         &tkp,
                         0,
                         (PTOKEN_PRIVILEGES)NULL,
                         0);

    if (GetLastError() != ERROR_SUCCESS) {
        printf("Error: AdjustTokenPrivileges failed (%d)\n", GetLastError());
        return 1;
    }

    // 重启系统
    if (!ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0)) {
        printf("Error: ExitWindowsEx failed (%d)\n", GetLastError());
        return 1;
    }

    printf("System rebooting...\n");
    return 0;
}