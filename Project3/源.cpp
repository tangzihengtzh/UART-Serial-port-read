#include <windows.h>
#include <stdio.h>

int main()
{
    HANDLE hSerial;
    DCB dcbSerialParams = { 0 };
    COMMTIMEOUTS timeouts = { 0 };
    DWORD bytesRead;
    char data[100];

    // �򿪴���
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
            printf("ɨ�赽���õĴ��ڣ�COM%d\n", i);
            CloseHandle(hSerial);
        }
    }
    printf("������ָ�����ڱ�ţ��س���ȷ�ϣ�\n");
    scanf_s("%d", &com_idx);
    printf("�����벨���ʣ��س���ȷ�ϣ�\n");
    scanf_s("%d", &bundrate);
    printf("��ʼ��ȡ...\n");
    hSerial = CreateFile(comlist[com_idx], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);



    // ���ô��ڲ���
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        printf("�޷���ȡ���ڲ���\n");
        CloseHandle(hSerial);
        return 1;
    }
    dcbSerialParams.BaudRate = bundrate; // ���ò�����
    dcbSerialParams.ByteSize = 8; // ��������λΪ8λ
    dcbSerialParams.Parity = NOPARITY; // ����У��λΪ��У��
    dcbSerialParams.StopBits = ONESTOPBIT; // ����ֹͣλΪ1��ֹͣλ
    if (!SetCommState(hSerial, &dcbSerialParams)) {
        printf("�޷����ô��ڲ���\n");
        CloseHandle(hSerial);
        return 1;
    }

    // ���ö�ȡ��ʱʱ��
    timeouts.ReadIntervalTimeout = 1000;
    timeouts.ReadTotalTimeoutConstant = 1000;
    timeouts.ReadTotalTimeoutMultiplier = 1000;
    timeouts.WriteTotalTimeoutConstant = 1000;
    timeouts.WriteTotalTimeoutMultiplier = 1000;
    if (!SetCommTimeouts(hSerial, &timeouts)) {
        printf("�޷����ô��ڳ�ʱ\n");
        CloseHandle(hSerial);
        return 1;
    }

    // ��ȡ��������
    while (1) {
        if (!ReadFile(hSerial, data, sizeof(data) - 1, &bytesRead, NULL)) {
            printf("�޷���ȡ��������\n");
            CloseHandle(hSerial);
            return 1;
        }
        if (bytesRead > 0) {
            data[bytesRead] = '\0'; // ��ĩβ����ַ���������
            printf("%s", data);
        }
    }

    // �رմ���
    CloseHandle(hSerial);

    return 0;
}








