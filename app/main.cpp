/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       扫码软件
*******************************************************************************/
#include <QApplication>
#include <QTranslator>
#include "appqrcode.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator qtTran;
    qtTran.load(":/qt_zh_CN.qm");
    a.installTranslator(&qtTran);

    QTranslator qtBase;
    qtBase.load(":/qtbase_zh_CN.qm");
    a.installTranslator(&qtBase);

    AppQrcode w;
    w.show();

    return a.exec();
}
