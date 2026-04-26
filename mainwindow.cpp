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

QString iwadDir;
QString modDir;
QJsonArray sourcePorts;

QString selPort;
QString selIwad;
QStringList selMods;
QString portArgs;

int MainWindow::readJson(QString path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox m;
        m.critical(0,"Error",path+" not found!");
        return 1;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (doc.isObject()) {
        QJsonObject obj = doc.object();

        iwadDir = obj["iwadDir"].toString();
        modDir = obj["modDir"].toString();
        sourcePorts = obj["sourcePorts"].toArray();

        if (iwadDir == "" || modDir == "" || sourcePorts.count() == 0) {
            QMessageBox m;
            m.critical(0,"Error", "Please specify iwadDir, modDir and sourcePorts.");
            return 1;
        }
    } else {
        QMessageBox m;
        m.critical(0,"Error", path+" is in invalid format.");
        return 1;
    }

    return 0;
}

QStringList getDirItems(QString path) {
    QDir directory(path);
    QStringList list = directory.entryList();
    list.remove(0,2);
    return list;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    readJson("search_dirs.json");
    ui->setupUi(this);
    ui->iwadCombo->addItems(getDirItems(iwadDir));
    foreach (const QJsonValue value, sourcePorts) {
        ui->spCombo->addItem(value.toString());
    }
    ui->modList->addItems(getDirItems(modDir));
    selPort = ui->spCombo->currentText();
    selIwad = ui->iwadCombo->currentText();
    foreach (const QListWidgetItem* value, ui->modList->selectedItems()) {
        selMods.append(value->text());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_playButton_clicked()
{
    QProcess process;
    QStringList args;
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


void MainWindow::on_spCombo_currentTextChanged(const QString &arg1)
{
    selPort = arg1;
}


void MainWindow::on_iwadCombo_currentTextChanged(const QString &arg1)
{
    selIwad = arg1;
}


void MainWindow::on_argsEdit_textChanged(const QString &arg1)
{
    portArgs = arg1;
}


void MainWindow::on_modList_itemSelectionChanged()
{
    selMods.clear();
    foreach (const QListWidgetItem* value, ui->modList->selectedItems()) {
        selMods.append(value->text());
    }
}

