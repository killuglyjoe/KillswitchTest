#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QHostAddress>

const QString cMaskIP("192.168.2.0/24");//check subnet for adapter

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_qaxWrapper(QSharedPointer<QWMIWrapper>(new QWMIWrapper))
{
    Q_ASSERT(!m_qaxWrapper.isNull());
    ui->setupUi(this);

    m_ifaceList = m_qaxWrapper->getWmiInterObjs(
                                "SELECT * FROM Win32_NetworkAdapterConfiguration"
                                 " WHERE IPEnabled=\"true\"");

    foreach(QAxObject *axObj, m_ifaceList)
    {
        QHostAddress ipv4(axObj->dynamicCall("IPAddress")
                          .toList().at(0).toString());

        if(ipv4.isInSubnet(QHostAddress::parseSubnet(cMaskIP)))
        {
            QString ifaceIdx(axObj->dynamicCall("Index").toString());
            if(!m_adaptersIndxList.contains(ifaceIdx))
                m_adaptersIndxList.append(ifaceIdx);
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
    foreach (QString adapterIdx, m_adaptersIndxList)
    {
        QList<QAxObject*> objs(m_qaxWrapper->getWmiInterObjs(
                                "SELECT * FROM Win32_NetworkAdapter WHERE "
                                "DeviceID=\'"+adapterIdx+"\'"));
        if(objs.count())
            objs.at(0)->dynamicCall(checked ? "Disable" : "Enable");
    }
}
