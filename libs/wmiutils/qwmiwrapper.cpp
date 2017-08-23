#include "qwmiwrapper.h"

#include <qt_windows.h>

#include <QDebug>
#include <QUuid>

QWMIWrapper::QWMIWrapper(QObject *parent) :
    QObject(parent),
    m_objIWbemLocator(QSharedPointer<QAxObject>
                      (
                          new QAxObject("WbemScripting.SWbemLocator"))
                      )
{
    Q_ASSERT_X(!m_objIWbemLocator->isNull(), Q_FUNC_INFO,
               "Error allocate SWbemLocator object");

    m_objWMIService = QSharedPointer<QAxObject>(
                m_objIWbemLocator->querySubObject(
                    "ConnectServer(QString&,QString&)",
                    QString("."), QString("root\\cimv2")
                )
            );

    if (m_objWMIService->isNull()) {
      qDebug() << Q_FUNC_INFO << "WMIService not created";
    }
}

QWMIWrapper::~QWMIWrapper()
{
}

QList<QAxObject*> QWMIWrapper::getWmiInterObjs(const QString &query)
{
    QList<QAxObject*> items;

    QAxObject *objInterList (getObjWMIService()->querySubObject(
          "ExecQuery(QString&))", query));
    if (!objInterList)
    {
        qDebug() << "Answer from ExecQuery is null";
        return items;
    }

    int count(0);
    if ((count = objInterList->dynamicCall("Count").toInt()) == 0)
    {
        qDebug() << "Count is 0";
        return items;
    }

    // И теперь самое интересное
    QAxObject *enum1 = objInterList->querySubObject("_NewEnum");
    if (!enum1) {
        qDebug() << "Query _NewEnum failed";
        return items;
    }

    IEnumVARIANT* enumInterface(0);
    enum1->queryInterface(IID_IEnumVARIANT, (void**)&enumInterface);
    if (!enumInterface) {
        qDebug() << "Query interface failed";
        return items;
    }

    enumInterface->Reset();

    VARIANT *theItem = new VARIANT;
    for (int i = 0; i < objInterList->dynamicCall("Count").toInt(); i++)
    {
        if (enumInterface->Next(1,theItem,NULL) == S_FALSE) {
            qDebug() << "enum next failed";
            delete theItem;
            return items;
        }

        items.append(new QAxObject((IUnknown*)theItem->punkVal));
        if (!items.count())
        {
            qDebug() << "getting result item failed";
            delete theItem;
            return items;
        }
        delete theItem;
    }

    return items;
}

QAxObject* QWMIWrapper::getObjWMIService() const
{
    return m_objWMIService.data();
}
