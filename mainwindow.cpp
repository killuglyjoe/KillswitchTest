#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_wmiHelper(QSharedPointer<WMIHelper>(new WMIHelper))
{
    Q_ASSERT(m_wmiHelper->hasErrror() == false);
    ui->setupUi(this);

    m_wmiHelper->createInstance("Win32_networkAdapter", "Disable");
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
    m_wmiHelper->excecCommandWithParams("notepad.exe", checked ? "par1" : "par2");
}
