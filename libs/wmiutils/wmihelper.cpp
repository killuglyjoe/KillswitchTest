#include "wmihelper.h"

wchar_t*  converToWChar_t(QString text)
{
    return (wchar_t*)(text.utf16());
}

WMIHelper::WMIHelper(QObject *parent,
                     const QString &className,
                     const QString &methodName) :
    QObject(parent),
    m_pLoc(0),
    m_pSvc(0),
    m_pClass(0),
    m_pInParamsDefinition(0),
    m_pClassInstance(0),
    m_hasErrror(false),
    m_MethodName(0),
    m_ClassName(0),
    m_errorString("")
{
    HRESULT hres;

    // WMI init routines taken from
    // https://msdn.microsoft.com/en-us/library/aa390421(v=vs.85).aspx

    // Step 1: --------------------------------------------------
    // Initialize COM. ------------------------------------------

    hres =  CoInitializeEx(0, COINIT_APARTMENTTHREADED);
    if (FAILED(hres))
    {
        cout << "Failed to initialize COM library. Error code = 0x"
             << hex << hres << endl;
        m_hasErrror = true;
        m_errorString = QString().sprintf("Failed to initialize COM library. "
                                "Error code = 0x%x",hres);
        return;
    }

    // Step 2: --------------------------------------------------
    // Set general COM security levels --------------------------

    hres =  CoInitializeSecurity(
        NULL,
        -1,                          // COM negotiates service
        NULL,                        // Authentication services
        NULL,                        // Reserved
        RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
        RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
        NULL,                        // Authentication info
        EOAC_NONE,                   // Additional capabilities
        NULL                         // Reserved
        );


    if (FAILED(hres))
    {
        cout << "Failed to initialize security. Error code = 0x"
             << hex << hres << endl;
        free();
        m_hasErrror = true;
        m_errorString = QString().sprintf("Failed to initialize security. "
                                "Error code = 0x%x", hres);
        return;
    }

    // Step 3: ---------------------------------------------------
    // Obtain the initial locator to WMI -------------------------

    hres = CoCreateInstance(
        CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator, (LPVOID *) &m_pLoc);

    if (FAILED(hres))
    {
        cout << "Failed to create IWbemLocator object. "
             << "Err code = 0x"
             << hex << hres << endl;
        free();
        m_hasErrror = true;
        m_errorString = QString().sprintf("Failed to create IWbemLocator object. "
                                "Error code = 0x%x", hres);
        return;
    }

    // Step 4: ---------------------------------------------------
    // Connect to WMI through the IWbemLocator::ConnectServer method


    // Connect to the local root\cimv2 namespace
    // and obtain pointer m_pSvc to make IWbemServices calls.
    hres = m_pLoc->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"),
        NULL,
        NULL,
        0,
        NULL,
        0,
        0,
        &m_pSvc
    );

    if (FAILED(hres))
    {
        cout << "Could not connect. Error code = 0x"
             << hex << hres << endl;
        free();
        m_hasErrror = true;
        m_errorString = QString().sprintf("Could not connect. "
                                "Error code = 0x%x", hres);
        return;
    }

    cout << "Connected to ROOT\\CIMV2 WMI namespace" << endl;

    // Step 5: --------------------------------------------------
    // Set security levels for the proxy ------------------------

    hres = CoSetProxyBlanket(
        m_pSvc,                        // Indicates the proxy to set
        RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
        RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
        NULL,                        // Server principal name
        RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
        RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
        NULL,                        // client identity
        EOAC_NONE                    // proxy capabilities
    );

    if (FAILED(hres))
    {
        cout << "Could not set proxy blanket. Error code = 0x"
             << hex << hres << endl;
        free();
        m_hasErrror = true;
        m_errorString = QString().sprintf("Could not set proxy blanket. "
                                "Error code = 0x%x", hres);
        return;
    }

    createInstance(className, methodName);
}

WMIHelper::~WMIHelper()
{
    qDebug() << Q_FUNC_INFO ;
    free();
}
/**
 * @brief Provides routines to
 * any methods of any WMI class
 * @param className
 * @param methodName
 */
void WMIHelper::createInstance(const QString &className, const QString &methodName)
{
    if(m_ClassName)             SysFreeString(m_ClassName);
    if(m_MethodName)            SysFreeString(m_MethodName);

    if(m_pClassInstance)
    {
        m_pClassInstance->Release();
        m_pClassInstance = 0;
    }
    if(m_pInParamsDefinition)
    {
        m_pInParamsDefinition->Release();
        m_pInParamsDefinition = 0;
    }

    m_MethodName = SysAllocString(converToWChar_t(methodName));
    m_ClassName = SysAllocString(converToWChar_t(className));

    HRESULT hres;

    // Step 6: --------------------------------------------------
    // Use the IWbemServices pointer to make requests of WMI ----

    hres = m_pSvc->GetObject(m_ClassName, 0, NULL, &m_pClass, NULL);
    if (FAILED(hres))
    {
        cout << "Failed to get class object. Error code = 0x"
             << hex << hres << endl;
//        free();
        m_hasErrror = true;
        m_errorString = QString().sprintf("Failed to get class object. "
                                "Error code = 0x%x", hres);
        return;
    }

    hres = m_pClass->GetMethod(m_MethodName, 0, &m_pInParamsDefinition, NULL);

    if(m_pInParamsDefinition == 0)
    {
        cout << "No any parameters need for method."<< endl;
//        m_pInParamsDefinition->Release();
        m_hasErrror = true;
        m_errorString = QString("No any parameters need for method.");
        return;
    }

    if (FAILED(hres))
    {
        cout << "No such method for this class. Error code = 0x"
             << hex << hres << endl;
//        free();
        m_hasErrror = true;
        m_errorString = QString().sprintf("No such method or this class. "
                                "Error code = 0x%x", hres);
        return;
    }

    hres = m_pInParamsDefinition->SpawnInstance(0, &m_pClassInstance);

    cout << "Succesfully created connection to "
         << className.toStdString() <<" with method "<< methodName.toStdString() << endl;
}
/**
 * @brief Execute any
 * @param command
 * @param params
 */
void WMIHelper::excecCommandWithParams(const QString &command,
                                       const QString &params)
{
    HRESULT hres;
    // Create the values for the in parameters
    VARIANT varCommand;
    varCommand.vt = VT_BSTR;
    varCommand.bstrVal = _bstr_t(converToWChar_t(command));

    // Store the value for the in parameters
    hres = m_pClassInstance->Put(L"CommandLine", 0,
        &varCommand, 0);
    wprintf(L"The command is: %s\n", V_BSTR(&varCommand));

    // Execute Method
    IWbemClassObject* pOutParams = NULL;
    hres = m_pSvc->ExecMethod(m_ClassName, m_MethodName, 0,
    NULL, m_pClassInstance, &pOutParams, NULL);

    if (FAILED(hres))
    {
        cout << "Could not execute method. Error code = 0x"
             << hex << hres << endl;
        VariantClear(&varCommand);
        pOutParams->Release();
        m_hasErrror = true;
        m_errorString = QString().sprintf("Could not execute method. "
                                "Error code = 0x%x", hres);
        return;
    }

    // To see what the method returned,
    // use the following code.  The return value will
    // be in &varReturnValue
    VARIANT varReturnValue;
    hres = pOutParams->Get(_bstr_t(L"ReturnValue"), 0,
        &varReturnValue, NULL, 0);

    // Clean up
    //--------------------------
    VariantClear(&varCommand);
    VariantClear(&varReturnValue);
    pOutParams->Release();
}

bool WMIHelper::hasErrror() const
{
    return m_hasErrror;
}

QString WMIHelper::errorString() const
{
    return m_errorString;
}

void WMIHelper::free()
{
    if(m_ClassName)             SysFreeString(m_ClassName);
    if(m_MethodName)            SysFreeString(m_MethodName);

    if(m_pClassInstance)        m_pClassInstance->Release();
    if(m_pInParamsDefinition)   m_pInParamsDefinition->Release();
    if(m_pClass)                m_pClass->Release();
    if(m_pSvc)                  m_pSvc->Release();
    if(m_pLoc)                  m_pLoc->Release();

    CoUninitialize();
}
