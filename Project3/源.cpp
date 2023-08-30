#include <windows.h>
#include <stdio.h>

int main()
{
    HANDLE hSerial;
    DCB dcbSerialParams = { 0 };
    COMMTIMEOUTS timeouts = { 0 };
    DWORD bytesRead;
    char data[100];

    // 打开串口
    const wchar_t comlist[10][5] = {
        L"COM0",
        L"COM1",
        L"COM2",
        L"COM3",
        L"COM4",
        L"COM5",
        L"COM6",
        L"COM7",
        L"COM8",
        L"COM9"
    };
    int com_idx;
    int bundrate = 9600;
    int comlist_available[10] = { 0,0,0,0,0,0,0,0,0,0 };
    for (int i = 0; i < 10; i++)
    {
        hSerial = CreateFile(comlist[i], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hSerial != INVALID_HANDLE_VALUE) {
            comlist_available[i] = 1;
            printf("扫描到可用的串口：COM%d\n", i);
            CloseHandle(hSerial);
        }
    }
    printf("请输入指定串口编号（回车键确认）\n");
    scanf_s("%d", &com_idx);
    printf("请输入波特率（回车键确认）\n");
    scanf_s("%d", &bundrate);
    printf("开始读取...\n");
    hSerial = CreateFile(comlist[com_idx], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);



    // 配置串口参数
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        printf("无法获取串口参数\n");
        CloseHandle(hSerial);
        return 1;
    }
    dcbSerialParams.BaudRate = bundrate; // 设置波特率
    dcbSerialParams.ByteSize = 8; // 设置数据位为8位
    dcbSerialParams.Parity = NOPARITY; // 设置校验位为无校验
    dcbSerialParams.StopBits = ONESTOPBIT; // 设置停止位为1个停止位
    if (!SetCommState(hSerial, &dcbSerialParams)) {
        printf("无法设置串口参数\n");
        CloseHandle(hSerial);
        return 1;
    }

    // 设置读取超时时间
    timeouts.ReadIntervalTimeout = 1000;
    timeouts.ReadTotalTimeoutConstant = 1000;
    timeouts.ReadTotalTimeoutMultiplier = 1000;
    timeouts.WriteTotalTimeoutConstant = 1000;
    timeouts.WriteTotalTimeoutMultiplier = 1000;
    if (!SetCommTimeouts(hSerial, &timeouts)) {
        printf("无法设置串口超时\n");
        CloseHandle(hSerial);
        return 1;
    }

    // 读取串口数据
    while (1) {
        if (!ReadFile(hSerial, data, sizeof(data) - 1, &bytesRead, NULL)) {
            printf("无法读取串口数据\n");
            CloseHandle(hSerial);
            return 1;
        }
        if (bytesRead > 0) {
            data[bytesRead] = '\0'; // 在末尾添加字符串结束符
            printf("%s", data);
        }
    }

    // 关闭串口
    CloseHandle(hSerial);

    return 0;
}








