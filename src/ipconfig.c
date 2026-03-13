#include <windows.h>
#include <stdio.h>
#include <iphlpapi.h>
#include <icmpapi.h>

#pragma comment(lib, "iphlpapi.lib")

int main() {
    DWORD dwRetVal = 0;
    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = NULL;

    // 分配内存
    pAdapterInfo = (IP_ADAPTER_INFO *)malloc(sizeof(IP_ADAPTER_INFO));
    if (pAdapterInfo == NULL) {
        printf("Error: Out of memory\n");
        return 1;
    }

    // 获取适配器信息
    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
        free(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
        if (pAdapterInfo == NULL) {
            printf("Error: Out of memory\n");
            return 1;
        }
    }

    if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
        pAdapter = pAdapterInfo;
        while (pAdapter) {
            printf("\n适配器名称: %s\n", pAdapter->AdapterName);
            printf("适配器描述: %s\n", pAdapter->Description);
            printf("  MAC地址: ");
            for (UINT i = 0; i < pAdapter->AddressLength; i++) {
                if (i == (pAdapter->AddressLength - 1))
                    printf("%.2X\n", (int)pAdapter->Address[i]);
                else
                    printf("%.2X-", (int)pAdapter->Address[i]);
            }
            printf("  类型: %ld\n", pAdapter->Type);
            printf("  IPv4地址列表:\n");
            IP_ADDR_STRING *pIpAddrString = &pAdapter->IpAddressList;
            while (pIpAddrString) {
                printf("    IP地址: %s\n", pIpAddrString->IpAddress.String);
                printf("    子网掩码: %s\n", pIpAddrString->IpMask.String);
                pIpAddrString = pIpAddrString->Next;
            }
            printf("  网关: %s\n", pAdapter->GatewayList.IpAddress.String);
            printf("  DHCP服务器: %s\n", pAdapter->DhcpServer.IpAddress.String);
            printf("  是否启用DHCP: %s\n", (pAdapter->Flags & IP_ADAPTER_DHCP_ENABLED) ? "是" : "否");
            pAdapter = pAdapter->Next;
        }
    } else {
        printf("Error: GetAdaptersInfo failed with error: %d\n", dwRetVal);
        free(pAdapterInfo);
        return 1;
    }

    free(pAdapterInfo);
    return 0;
}