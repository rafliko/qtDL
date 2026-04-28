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

    void readJson(QString path);

    void writeJson(QString path, QString key, QString value);

    void writeJson(QString path, QString key, QStringList value);

    void update_ui();

    void on_actionAdd_Source_Port_triggered();

    void on_actionAdd_IWAD_dir_triggered();

    void on_actionChange_mod_dir_triggered();

    void on_actionClear_All_triggered();

private:
    Ui::MainWindow *ui;
    QString iwadDir;
    QString modDir;
    QJsonArray sourcePorts;
};
#endif // MAINWINDOW_H
