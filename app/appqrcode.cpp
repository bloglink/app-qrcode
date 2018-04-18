/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       扫码软件
*******************************************************************************/
#include "appqrcode.h"

AppQrcode::AppQrcode(QWidget *parent) : QMainWindow(parent)
{
    initUI();
}

AppQrcode::~AppQrcode()
{
}

void AppQrcode::initUI()
{
    initSkin();
    initTitle();
    initLayout();
    initInputs();
    initDisplay();
}

void AppQrcode::initSkin()
{
    QFile file;
    QString qss;
    file.setFileName(":/qss_black.css");
    file.open(QFile::ReadOnly);
    qss = QLatin1String(file.readAll());
    qApp->setStyleSheet(qss);
}

void AppQrcode::initTitle()
{
    char s_month[5];
    static const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
    int month, day, year;

    sscanf((__DATE__), "%s %d %d", s_month, &day, &year);
    month = (strstr(month_names, s_month)-month_names)/3+1;

    QDate dt;
    dt.setDate(year, month, day);
    static const QTime tt = QTime::fromString(__TIME__, "hh:mm:ss");

    QDateTime t(dt, tt);
    QString verNumb = QString("V-0.1.%1").arg(t.toString("yyMMdd-hhmm"));

    this->setWindowTitle(tr("青岛艾普扫码软件%1").arg(verNumb));
}

void AppQrcode::initLayout()
{
    layout = new QHBoxLayout;


    QFrame *frame = new QFrame(this);
    frame->setLayout(layout);

    this->setCentralWidget(frame);
    this->resize(800, 600);
}

void AppQrcode::initInputs()
{
    QStringList label;
    names << "hostname";
    label << "网络地址:";
    names << "username";
    label << "登录用户:";
    names << "password";
    label << "登录密码:";
    names << "database";
    label << "数据库名:";
    names << "codestart";
    label << "条码起始:";
    names << "codelenth";
    label << "条码长度:";

    QVBoxLayout *edLayout = new QVBoxLayout;
    for (int i=0; i < names.size(); i++) {
        QHBoxLayout *ll = new QHBoxLayout;
        ll->addWidget(new QLabel(label.at(i), this));
        QLineEdit *ed = new QLineEdit(this);
        ll->addWidget(ed);
        if (names.at(i) == "password")
            ed->setEchoMode(QLineEdit::Password);
        edLayout->addLayout(ll);
        inputs.append(ed);
    }
    edLayout->addStretch();

    input = new QGroupBox(this);
    input->setTitle(tr("参数设置区"));
    input->setLayout(edLayout);
    layout->addWidget(input);
}

void AppQrcode::initDisplay()
{
    QGroupBox *group = new QGroupBox(this);
    group->setTitle(tr("条码输入区"));
    layout->addWidget(group);

    QVBoxLayout *ipLayout = new QVBoxLayout;
    text = new QTextBrowser(this);
    ipLayout->addWidget(text);
    group->setLayout(ipLayout);

    codes = new QTextBrowser(this);
    codes->setFixedHeight(100);
    ipLayout->addWidget(codes);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    ipLayout->addLayout(btnLayout);
    btnLayout->addStretch();

    QPushButton *btnStart = new QPushButton(this);
    btnStart->setText(tr("启动扫码"));
    btnStart->setFixedSize(97, 44);
    btnLayout->addWidget(btnStart);
    connect(btnStart, SIGNAL(clicked(bool)), this, SLOT(clickButton()));

    layout->setStretch(1, 1);

    scanner = new QTimer(this);
    connect(scanner, SIGNAL(timeout()), this, SLOT(showBarcode()));
    isStart = false;
}

void AppQrcode::initSetting()
{
    QSettings *set = new QSettings("default.ini", QSettings::IniFormat);
    set->setIniCodec("GB18030");
    set->beginGroup("QRCODE");
    for (int i=0; i < names.size(); i++) {
        inputs.at(i)->setText(set->value(names.at(i)).toString());
    }
    set->endGroup();
    set->deleteLater();
}

void AppQrcode::saveSetting()
{
    QSettings *set = new QSettings("default.ini", QSettings::IniFormat);
    set->setIniCodec("GB18030");
    set->beginGroup("QRCODE");
    for (int i=0; i < names.size(); i++) {
        set->setValue(names.at(i), inputs.at(i)->text());
    }
    set->endGroup();
    set->deleteLater();
}

void AppQrcode::clickButton()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (btn->text() == tr("启动扫码")) {
        if (connectOdbc()) {
            btn->setText(tr("停止扫码"));
            input->setEnabled(false);
            codes->setFocus();
            isStart = true;
        }
    } else {
        btn->setText(tr("启动扫码"));
        input->setEnabled(true);
        isStart = false;
    }
}

bool AppQrcode::connectOdbc()
{
    QString dsn;
    QString driver;
    QString hostname = inputs.at(names.indexOf("hostname"))->text();
    QString database = inputs.at(names.indexOf("database"))->text();
    QString username = inputs.at(names.indexOf("username"))->text();
    QString password = inputs.at(names.indexOf("password"))->text();
    driver = "QODBC3";
    dsn = QString("DRIVER={SQL SERVER};SERVER=%1;DATABASE=%2;").arg(hostname).arg(database);

    QSqlDatabase db = QSqlDatabase::addDatabase(driver, "record");
    db.setHostName(hostname);
    db.setUserName(username);
    db.setPassword(password);
    db.setDatabaseName(dsn);
    db.setConnectOptions("SQL_ATTR_LOGIN_TIMEOUT=2;SQL_ATTR_CONNECTION_TIMEOUT=2");
    if (!db.open()) {
        qDebug() << db.lastError().text();
        return false;
    }
    sqlTab = new QSqlTableModel(this, db);
    sqlTab->setTable("aip_record");
    sqlTab->select();
    qDebug() << sqlTab->rowCount();
    return true;
}

void AppQrcode::showBarcode()
{
    scanner->stop();
    if (barcode.size() > 2)
        codes->setText(barcode);
    int s = inputs.at(names.indexOf("codestart"))->text().toInt();
    int t = inputs.at(names.indexOf("codelenth"))->text().toInt();
    QString c = barcode.mid(s, t);
    QString r = "NO";

    QSqlQuery query(QSqlDatabase::database("record"));
    QString cmd = tr("select numb,pass from aip_record");
    query.exec(cmd);
    while (query.next()) {
        if (query.value(0).toString() == c) {
            r = query.value(1).toString();
            break;
        }
    }

    QString base;
    if (r == "OK")
        base += "<font color='#00FF00'>";
    if (r == "NG")
        base += "<font color='#FF0000'>";
    if (r == "NO")
        base += "<font color='#00FFFF'>";
    base += "<p style='font-size:320px' align='center'><b>%1</b></p></font>";
    text->setText(base.arg(r));
    barcode.clear();
}

void AppQrcode::keyPressEvent(QKeyEvent *e)
{
    e->accept();
    if (!isStart)
        return;
    this->setFocus();
    scanner->stop();
    barcode.append(e->text());
}

void AppQrcode::keyReleaseEvent(QKeyEvent *e)
{
    e->accept();
    if (!isStart)
        return;
    scanner->start(500);
}

void AppQrcode::showEvent(QShowEvent *e)
{
    initSetting();
    e->accept();
}

void AppQrcode::hideEvent(QHideEvent *e)
{
    saveSetting();
    e->accept();
}
