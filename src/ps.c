#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>
#include <tchar.h>

int main() {
    HANDLE hProcessSnap;
    HANDLE hProcess;
    PROCESSENTRY32 pe32;
    DWORD dwPriorityClass;

    // 获取进程快照
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        printf("Error: CreateToolhelp32Snapshot failed (%d)\n", GetLastError());
        return 1;
    }

    // 设置结构体大小
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // 获取第一个进程
    if (!Process32First(hProcessSnap, &pe32)) {
        printf("Error: Process32First failed (%d)\n", GetLastError());
        CloseHandle(hProcessSnap);
        return 1;
    }

    // 打印表头
    printf("PID   PPID  Name\n");
    printf("===== ==== =========================================\n");

    // 遍历所有进程
    do {
        printf("%5d %4d %-40s\n", 
               pe32.th32ProcessID, 
               pe32.th32ParentProcessID, 
               pe32.szExeFile);
    } while (Process32Next(hProcessSnap, &pe32));

    // 清理
    CloseHandle(hProcessSnap);
    return 0;
}