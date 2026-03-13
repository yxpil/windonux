#include <windows.h>
#include <stdio.h>

int main() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) {
        return 1;
    }

    // 获取控制台信息
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        return 1;
    }

    // 计算要清除的字符数
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    // 填充空格
    COORD coordScreen = { 0, 0 };
    DWORD cCharsWritten;
    if (!FillConsoleOutputCharacter(hConsole, ' ', dwConSize, coordScreen, &cCharsWritten)) {
        return 1;
    }

    // 获取当前属性并设置回控制台
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        return 1;
    }

    if (!FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten)) {
        return 1;
    }

    // 将光标移回左上角
    if (!SetConsoleCursorPosition(hConsole, coordScreen)) {
        return 1;
    }

    return 0;
}