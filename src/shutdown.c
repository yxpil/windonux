#include <windows.h>
#include <stdio.h>
#include <tchar.h>

int main(int argc, char *argv[]) {
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;
    BOOL force = FALSE;
    DWORD timeout = 0; // 立即关机

    // 解析命令行参数
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) {
            force = TRUE;
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            timeout = atoi(argv[++i]) * 1000; // 转换为毫秒
        }
    }

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

    // 如果有超时，等待指定时间
    if (timeout > 0) {
        printf("System shutting down in %d seconds...\n", timeout / 1000);
        Sleep(timeout);
    }

    // 关闭系统
    UINT flags = EWX_SHUTDOWN;
    if (force) {
        flags |= EWX_FORCE;
    }

    if (!ExitWindowsEx(flags, 0)) {
        printf("Error: ExitWindowsEx failed (%d)\n", GetLastError());
        return 1;
    }

    printf("System shutting down...\n");
    return 0;
}