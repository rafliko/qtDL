#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonArray>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_playButton_clicked();

    QStringList getDirItems(QString path);

    int readJson(QString path);

    void on_actionAdd_Source_Port_triggered();

private:
    Ui::MainWindow *ui;
    QString iwadDir;
    QString modDir;
    QJsonArray sourcePorts;
};
#endif // MAINWINDOW_H
