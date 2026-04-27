#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QProcess>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QDir>
#include <QMessageBox>

int MainWindow::readJson(QString path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox m;
        m.critical(0,"Error",path+" not found!");
        return -1;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (doc.isObject()) {
        QJsonObject obj = doc.object();

        this->iwadDir = obj["iwadDir"].toString();
        this->modDir = obj["modDir"].toString();
        this->sourcePorts = obj["sourcePorts"].toArray();

        if (this->iwadDir == "" || this->modDir == "" || this->sourcePorts.count() == 0) {
            QMessageBox m;
            m.critical(0,"Error", "Please specify iwadDir, modDir and sourcePorts.");
            return -1;
        }
    } else {
        QMessageBox m;
        m.critical(0,"Error", path+" is in invalid format.");
        return -1;
    }

    return 0;
}

QStringList MainWindow::getDirItems(QString path) {
    QDir directory(path);
    QStringList list = directory.entryList();
    list.remove(0,2);
    return list;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if(readJson(":/search_dirs.json")!=0) exit(1);

    ui->iwadCombo->addItems(getDirItems(this->iwadDir));
    foreach (const QJsonValue value, this->sourcePorts) {
        ui->spCombo->addItem(value.toString());
    }
    ui->modList->addItems(getDirItems(modDir));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_playButton_clicked()
{
    QProcess process;
    QStringList args;

    QString selPort = this->ui->spCombo->currentText();
    QString selIwad = this->ui->iwadCombo->currentText();
    QStringList selMods;
    foreach (const QListWidgetItem* value, this->ui->modList->selectedItems()) {
        selMods.append(value->text());
    }
    QString portArgs = this->ui->argsEdit->text();

    args << "-iwad" << iwadDir+"/"+selIwad;
    if (portArgs.length() > 0) {
        foreach (QString a, portArgs.split(" ")) {
            args << a;
        }
    }
    if (selMods.length() > 0) {
        foreach (QString mod, selMods) {
            args << "-file" << modDir+"/"+mod;
        }
    }
    foreach (QString a, args) {
        qDebug() << a;
    }
    process.startDetached(selPort, args);
    process.waitForFinished();
}

void MainWindow::on_actionAdd_Source_Port_triggered()
{

}

