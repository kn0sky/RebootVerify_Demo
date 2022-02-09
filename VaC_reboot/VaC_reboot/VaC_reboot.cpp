#include "VaC_reboot.h"
#include <QCommandLineParser>
VaC_reboot::VaC_reboot(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    verifyKey = 0;
    Verify();
    connect(ui.pushButton, &QPushButton::clicked, this, &VaC_reboot::RebootVerify);


}
void VaC_reboot::RebootVerify() {
    HANDLE hFile = CreateFileW(L"Serial.txt", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    char writeBuf[0x100] = { 0 };
    strcpy(writeBuf, ui.lineEdit_UserName->text().toStdString().c_str());
    strcat(writeBuf, ":");
    strcat(writeBuf, ui.lineEdit_Serial->text().toStdString().c_str());
    DWORD dwRet;
    WriteFile(hFile, writeBuf, strlen(writeBuf), &dwRet, NULL);
    CloseHandle(hFile);

    QMessageBox::about(this, "Information", "Reboot to Verify User Serial");

    // ��������
    // ��ȡ����·��
    char filePath[MAX_PATH] = { 0 };
    GetModuleFileNameA(NULL, filePath, MAX_PATH);
    // ��������
    PROCESS_INFORMATION pi = { 0 };
    STARTUPINFOA si = { 0 };
    si.cb = sizeof(STARTUPINFOA);
    BOOL bRet = CreateProcessA(filePath,
        NULL, NULL, NULL, FALSE,
        NULL, NULL, NULL, &si, &pi);
    // �˳���ǰ����
    ExitProcess(1);
}

std::string VaC_reboot::CalcSerial(std::string Uname)
{
    std::string UserName = Uname;
    std::string Serial;

    for(auto a:UserName)
    {
        verifyKey += a;
    }

    Serial = "VaC_";
    Serial += UserName[0] ^ verifyKey;
    for (int i = 1; UserName[i]; i++) {
        Serial += (UserName[i - 1] ^ UserName[i])%10 + '0';
    }
    return Serial;
}

void VaC_reboot::Verify()
{
    // ���ļ�
    HANDLE hFile = CreateFileW(L"Serial.txt", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    // ��ȡ�ļ�
    char buf[0x100] = { 0 };
    DWORD readSize = 0;
    ReadFile(hFile, buf, 0x100, &readSize, NULL);
    if (readSize == 0) {
        CloseHandle(hFile);
        return;
    }
    CloseHandle(hFile);

    std::string sbuf = buf;
    std::string Uname = sbuf.substr(0, sbuf.find(':'));
    std::string USerial = sbuf.substr(sbuf.find(':')+1, sbuf.length());
    std::string RealSerial = CalcSerial(Uname);

    // ����ļ���ȡ�������ˣ��ȶ����к��Ƿ���ȷ�����������������
    if (readSize) {
        if (!strcmp(USerial.c_str(), RealSerial.c_str())) {
            QMessageBox::about(this, "Error", "Verify Success!");
            ExitProcess(0);
        }
        else {
            QMessageBox::about(this, "Error", "Verify Failed!");
        }
    }

}
