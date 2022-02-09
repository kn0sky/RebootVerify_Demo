#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_VaC_reboot.h"
#include <Windows.h>
#include <QMessageBox>

class VaC_reboot : public QMainWindow
{
    Q_OBJECT

public:
    VaC_reboot(QWidget *parent = Q_NULLPTR);
    std::string CalcSerial(std::string Uname);

private:
    char verifyKey;

public slots:
    void RebootVerify();
    void Verify();
private:
    Ui::VaC_rebootClass ui;
};
