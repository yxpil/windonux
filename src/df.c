#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <string.h>

// 格式化字节大小为易读格式
void formatBytes(ULARGE_INTEGER bytes, TCHAR* buffer, DWORD bufferSize) {
    const TCHAR* units[] = { TEXT("B"), TEXT("KB"), TEXT("MB"), TEXT("GB"), TEXT("TB") };
    int unitIndex = 0;
    double size = (double)bytes.QuadPart;

    while (size >= 1024 && unitIndex < 4) {
        size /= 1024;
        unitIndex++;
    }

    _stprintf(buffer, TEXT("%.1f %s"), size, units[unitIndex]);
}

int main() {
    DWORD drives = GetLogicalDrives();
    DWORD driveIndex = 0;
    BOOL showHeader = TRUE;

    while (drives) {
        if (drives & 1) {
            TCHAR driveLetter[4];
            _stprintf(driveLetter, TEXT("%c:\\\"), TEXT('A') + driveIndex);

            DWORD driveType = GetDriveType(driveLetter);
            if (driveType == DRIVE_FIXED || driveType == DRIVE_REMOVABLE || driveType == DRIVE_REMOTE) {
                ULARGE_INTEGER totalBytes, freeBytes;
                if (GetDiskFreeSpaceEx(driveLetter, NULL, &totalBytes, &freeBytes)) {
                    ULARGE_INTEGER usedBytes;
                    usedBytes.QuadPart = totalBytes.QuadPart - freeBytes.QuadPart;

                    TCHAR totalStr[20], usedStr[20], freeStr[20];
                    formatBytes(totalBytes, totalStr, sizeof(totalStr));
                    formatBytes(usedBytes, usedStr, sizeof(usedStr));
                    formatBytes(freeBytes, freeStr, sizeof(freeStr));

                    // 计算使用率
                    DWORD percentUsed = (totalBytes.QuadPart > 0) ? 
                        (DWORD)((usedBytes.QuadPart * 100) / totalBytes.QuadPart) : 0;

                    // 打印表头
                    if (showHeader) {
                        printf("Drive  Total    Used     Free     Use%%  Type\n");
                        printf("===== ======== ======== ======== ===== =======\n");
                        showHeader = FALSE;
                    }

                    // 获取驱动器类型名称
                    const TCHAR* typeName;
                    switch (driveType) {
                        case DRIVE_FIXED: typeName = TEXT("Fixed"); break;
                        case DRIVE_REMOVABLE: typeName = TEXT("Removable"); break;
                        case DRIVE_REMOTE: typeName = TEXT("Network"); break;
                        default: typeName = TEXT("Other"); break;
                    }

                    // 打印驱动器信息
                    _tprintf(TEXT("%s    %-8s %-8s %-8s %3d%%  %s\n"),
                            driveLetter, totalStr, usedStr, freeStr, percentUsed, typeName);
                }
            }
        }

        drives >>= 1;
        driveIndex++;
    }

    if (showHeader) {
        printf("No drives found or accessible.\n");
        return 1;
    }

    return 0;
}