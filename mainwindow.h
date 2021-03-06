#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

//#include "libs/wmiutils/wmihelper.h"
#include "qwmiwrapper.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_checkBox_clicked(bool checked);

private:
    Ui::MainWindow              *ui;
//    QSharedPointer<WMIHelper>   m_wmiHelper;
    QSharedPointer<QWMIWrapper> m_qaxWrapper;
    QStringList                 m_adaptersIndxList;

    QList<QAxObject*>           m_ifaceList;
};

#endif // MAINWINDOW_H
