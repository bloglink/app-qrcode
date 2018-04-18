/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       扫码软件
*******************************************************************************/
#ifndef APPQRCODE_H
#define APPQRCODE_H

#include <QFile>
#include <QLabel>
#include <QTimer>
#include <QDebug>
#include <QLayout>
#include <QKeyEvent>
#include <QLineEdit>
#include <QDateTime>
#include <QGroupBox>
#include <QTextEdit>
#include <QPushButton>
#include <QMainWindow>
#include <QApplication>
#include <QTextBrowser>
#include <QStackedWidget>
#include <QSettings>
#include <QMessageBox>

#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlRecord>

class AppQrcode : public QMainWindow
{
    Q_OBJECT

public:
    explicit AppQrcode(QWidget *parent = 0);
    ~AppQrcode();
private slots:
    void initUI();
    void initSkin();
    void initTitle();
    void initLayout();
    void initInputs();
    void initDisplay();
    void initSetting();
    void saveSetting();
    void clickButton();
    bool connectOdbc();
    void showBarcode();
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void keyReleaseEvent(QKeyEvent *e);
    virtual void showEvent(QShowEvent *e);
    virtual void hideEvent(QHideEvent *e);
private:
    QHBoxLayout *layout;
    QList<QLineEdit*> inputs;
    QStringList names;

    QGroupBox *input;

    QTextBrowser *text;
    QTextBrowser *codes;

    QTimer *scanner;
    QString barcode;
    bool isStart;
    QSqlTableModel *sqlTab;
};

#endif // APPQRCODE_H
