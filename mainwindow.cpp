#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QProcess>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QDir>
#include <QFileDialog>

#define JSON_PATH QCoreApplication::applicationDirPath()+"/search_dirs.json"

void MainWindow::readJson(QString path) {
    QFile file(path);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray data = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject obj = doc.object();

        this->sourcePorts = obj["sourcePorts"].toArray();
        this->iwadDir = obj["iwadDir"].toString();
        this->modDir = obj["modDir"].toString();

        file.close();
    } else {
        writeJson(JSON_PATH, "sourcePorts", QStringList());
        writeJson(JSON_PATH, "iwadDir", "");
        writeJson(JSON_PATH, "modDir", "");
    }
}

void MainWindow::writeJson(QString path, QString key, QString value) {
    QFile file(path);
    QJsonObject obj;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray data = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        obj = doc.object();
        file.close();
    }

    obj[key] = value;

    if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QJsonDocument doc(obj);
        file.write(doc.toJson());
        file.close();
    }
}

void MainWindow::writeJson(QString path, QString key, QStringList value) {
    QFile file(path);
    QJsonObject obj;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray data = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        obj = doc.object();
        file.close();
    }

    QJsonArray tmp = obj[key].toArray();
    if (!value.empty()) {
        foreach (QString s, value) {
            tmp.append(s);
        }
    }
    else tmp = QJsonArray();
    obj[key] = tmp;

    if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QJsonDocument doc(obj);
        file.write(doc.toJson());
        file.close();
    }
}

QStringList MainWindow::getDirItems(QString path) {
    QDir directory(path);
    QStringList list = directory.entryList();
    list.remove(0,2);
    return list;
}

void MainWindow::update_ui() {
    readJson(JSON_PATH);

    ui->spCombo->clear();
    ui->iwadCombo->clear();
    ui->modList->clear();

    if (sourcePorts.count() != 0) {
        foreach (const QJsonValue value, sourcePorts) {
            ui->spCombo->addItem(value.toString());
        }
    }
    if (iwadDir!="") ui->iwadCombo->addItems(getDirItems(iwadDir));
    if (modDir!="") ui->modList->addItems(getDirItems(modDir));
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    update_ui();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_playButton_clicked()
{
    QProcess process;
    QStringList args;

    QString selPort = ui->spCombo->currentText();
    QString selIwad = ui->iwadCombo->currentText();
    QStringList selMods;
    foreach (const QListWidgetItem* value, ui->modList->selectedItems()) {
        selMods.append(value->text());
    }
    QString portArgs = ui->argsEdit->text();

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
    QString path = QFileDialog::getOpenFileName(this);
    QStringList list(path);
    if (path != "") {
        writeJson(JSON_PATH, "sourcePorts", list);
    }
    update_ui();
}


void MainWindow::on_actionAdd_IWAD_dir_triggered()
{
    QString path = QFileDialog::getExistingDirectory(this);
    if (path != "") {
        writeJson(JSON_PATH, "iwadDir", path);
    }
    update_ui();
}


void MainWindow::on_actionChange_mod_dir_triggered()
{
    QString path = QFileDialog::getExistingDirectory(this);
    if (path != "") {
        writeJson(JSON_PATH, "modDir", path);
    }
    update_ui();
}

void MainWindow::on_actionClear_All_triggered()
{
    writeJson(JSON_PATH, "sourcePorts", QStringList());
    writeJson(JSON_PATH, "iwadDir", "");
    writeJson(JSON_PATH, "modDir", "");
    update_ui();
}

