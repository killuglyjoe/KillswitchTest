#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_wmiHelper(QSharedPointer<WMIHelper>(new WMIHelper))
{
    Q_ASSERT(m_wmiHelper->hasErrror() == false);
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief Killswitch state control
 * @param checked
 */
void MainWindow::on_checkBox_clicked(bool checked)
{
    Q_UNUSED(checked);
    m_wmiHelper->execQuery("SELECT * FROM Win32_NetworkAdapter WHERE "
                           "NetConnectionStatus=\'2\'",
                           "ProductName");
}
