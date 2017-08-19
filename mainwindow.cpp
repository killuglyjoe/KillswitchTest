#include "mainwindow.h"
#include "ui_mainwindow.h"

const QString cMaskIP("192.168.2");//check subnet for adapter (looks ugly, better QHostAddress::isSubnet)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_wmiHelper(QSharedPointer<WMIHelper>(new WMIHelper("Win32_NetworkAdapter")))
{
    Q_ASSERT(m_wmiHelper->hasErrror() == false);
    ui->setupUi(this);

    //
    QStringList adaptersList;
    m_wmiHelper->execQuery("SELECT * FROM Win32_NetworkAdapterConfiguration WHERE "
                           "IPEnabled=\"true\"","IPADDress", &adaptersList);
//    qDebug() << adaptersList;

    // I must have wrapper class around adapters config
    foreach (QString address, adaptersList)
    {
        if(address.contains(cMaskIP))
        {
            QStringList idxList;
            m_wmiHelper->execQuery("SELECT * FROM Win32_NetworkAdapterConfiguration WHERE "
                                   "IPEnabled=\"true\"","index", &idxList);
            if(idxList.isEmpty()) continue;
//            qDebug() << idxList;
            foreach (QString idx, idxList) {
                if(!m_adaptersIndxList.contains(idx))
                    m_adaptersIndxList.append(idx);
            }
        }
    }
//    qDebug() << m_adaptersIndxList;
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
    foreach (QString adapterIdx, m_adaptersIndxList) {
        m_wmiHelper->execQueryWithCommand("SELECT * FROM Win32_NetworkAdapter WHERE "
                               "DeviceID=\'"+adapterIdx+"\'",
                               "DeviceID", checked ? "Disable" : "Enable");
    }
}
