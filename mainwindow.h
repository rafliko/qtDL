#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

    static int readJson(QString path);

private slots:
    void on_playButton_clicked();

    void on_spCombo_currentTextChanged(const QString &arg1);

    void on_iwadCombo_currentTextChanged(const QString &arg1);

    void on_argsEdit_textChanged(const QString &arg1);

    void on_modList_itemSelectionChanged();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
