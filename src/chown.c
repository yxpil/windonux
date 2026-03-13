#include <windows.h>
#include <stdio.h>
#include <aclapi.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: chown username file [file...]
");
        return 1;
    }

    const char *username = argv[1];
    DWORD error = 0;
    int successCount = 0;
    int errorCount = 0;

    // 获取用户的SID
    PSID pUserSid = NULL;
    DWORD cbUserSid = 0;
    LPTSTR DomainName = NULL;
    DWORD cbDomainName = 0;
    SID_NAME_USE peUse;

    // 第一次调用获取所需的缓冲区大小
    LookupAccountNameA(NULL, username, NULL, &cbUserSid, NULL, &cbDomainName, &peUse);
    if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
        printf("Error: Cannot find user '%s' (%d)\n", username, GetLastError());
        return 1;
    }

    // 分配内存
    pUserSid = (PSID)malloc(cbUserSid);
    DomainName = (LPTSTR)malloc(cbDomainName * sizeof(TCHAR));
    if (pUserSid == NULL || DomainName == NULL) {
        printf("Error: Out of memory\n");
        if (pUserSid) free(pUserSid);
        if (DomainName) free(DomainName);
        return 1;
    }

    // 第二次调用获取用户SID
    if (!LookupAccountNameA(NULL, username, pUserSid, &cbUserSid, DomainName, &cbDomainName, &peUse)) {
        printf("Error: LookupAccountName failed (%d)\n", GetLastError());
        free(pUserSid);
        free(DomainName);
        return 1;
    }

    // 处理每个文件参数
    for (int i = 2; i < argc; i++) {
        PACL pOldDACL = NULL;
        PACL pNewDACL = NULL;
        PSECURITY_DESCRIPTOR pSD = NULL;
        EXPLICIT_ACCESS ea;

        // 获取文件的安全描述符
        if (!GetNamedSecurityInfoA(argv[i], SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, &pOldDACL, NULL, &pSD)) {
            // 设置新的所有者
            if (!SetNamedSecurityInfoA(argv[i], SE_FILE_OBJECT, OWNER_SECURITY_INFORMATION, pUserSid, NULL, NULL, NULL)) {
                printf("Error: Cannot change owner for '%s' (%d)\n", argv[i], GetLastError());
                errorCount++;
            } else {
                printf("Owner of '%s' changed to '%s'\n", argv[i], username);
                successCount++;
            }
        } else {
            printf("Error: Cannot get security info for '%s' (%d)\n", argv[i], GetLastError());
            errorCount++;
        }

        // 释放资源
        if (pSD) LocalFree(pSD);
    }

    // 释放资源
    free(pUserSid);
    free(DomainName);

    if (errorCount > 0) {
        return 1;
    }

    return 0;
}