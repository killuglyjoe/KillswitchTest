#ifndef WMIHELPER_H
#define WMIHELPER_H

#include <QObject>
#include <QDebug>

#include <iostream>
using namespace std;
#include <comdef.h>
#include <Wbemidl.h>

#pragma comment(lib, "wbemuuid.lib")

class WMIHelper : public QObject
{
    Q_OBJECT
public:
    explicit WMIHelper(QObject *parent = 0,
                       const QString &className = "Win32_Process",
                       const QString &methodName = "Create");
    ~WMIHelper();

    void createInstance(const QString &className = "Win32_Process",
                        const QString &methodName = "Create");

    void excecCommandWithParams(const QString &command,
                                const QString &params);

    bool hasErrror() const;

    QString errorString() const;

signals:

public slots:

protected:
    void free();

private:
    //WMI ptrs (TODO: use smart ptrs, maybe)
    IWbemLocator        *m_pLoc; //initial locator to WMI
    IWbemServices       *m_pSvc;
    IWbemClassObject    *m_pClass;
    IWbemClassObject    *m_pInParamsDefinition;
    IWbemClassObject    *m_pClassInstance;

    bool                m_hasErrror;

    // set up to call the Win32_Process::Create method
    BSTR                m_MethodName;
    BSTR                m_ClassName;

    QString             m_errorString;
};

#endif // WMIHELPER_H