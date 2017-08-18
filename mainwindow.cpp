#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    m_wmiHelper = new WMIHelper;
    Q_ASSERT(m_wmiHelper.hasErrror() == false);
    qDebug() << m_wmiHelper.hasErrror() << m_wmiHelper.errorString();
}

MainWindow::~MainWindow()
{
//    delete m_wmiHelper;
    delete ui;
}

/**
 * @brief Killswitch state control
 * @param checked
 */
void MainWindow::on_checkBox_clicked(bool checked)
{
    m_wmiHelper.excecCommandWithParams("", checked ? "" : "");
}
