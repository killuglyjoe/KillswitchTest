#ifndef QWMIWRAPPER_H
#define QWMIWRAPPER_H

#include <QObject>
#include <QAxObject>
#include <QSharedPointer>

class QWMIWrapper : public QObject
{
    Q_OBJECT
public:
    explicit QWMIWrapper(QObject *parent = nullptr);
    ~QWMIWrapper();

    QList<QAxObject*> getWmiInterObjs(const QString &name);

    QAxObject *getObjWMIService() const;

private:
    QSharedPointer<QAxObject>   m_objIWbemLocator;
    QSharedPointer<QAxObject>   m_objWMIService;
};

#endif // QWMIWRAPPER_H
