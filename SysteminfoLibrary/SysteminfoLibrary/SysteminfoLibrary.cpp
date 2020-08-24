#include "pch.h"
#include "SysteminfoLibrary.h"
#include <tchar.h>
#include <map>
#include <iostream>

#define _WIN32_DCOM
#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")

unsigned __int64 getTotalDiskSpace() {
	ULARGE_INTEGER y;
	GetDiskFreeSpaceExA(NULL, NULL, &y, NULL);
	return y.QuadPart;
}

void getDiskInfo(unsigned __int64* FreeBytesAvailableToCaller, unsigned __int64* TotalNumberOfBytes, unsigned __int64* TotalNumberOfFreeBytes) {
	ULARGE_INTEGER availableToCaller, total, totalFree;
	GetDiskFreeSpaceExA(NULL, &availableToCaller, &total, &totalFree);
	*FreeBytesAvailableToCaller = availableToCaller.QuadPart;
	*TotalNumberOfBytes = total.QuadPart;
	*TotalNumberOfFreeBytes = totalFree.QuadPart;
	return;
}

void getHostname(char hostname[], LPDWORD bufferSize) {
	GetComputerNameA(hostname, bufferSize);
	return;
}

void getOsVersion(TCHAR osVersion[], LPDWORD size) {
	RegGetValue(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), _T("ProductName"), RRF_RT_ANY, NULL, osVersion, size);
}

void getOsRegisteredOwner(TCHAR osRegisteredOwner[], LPDWORD size) {
	RegGetValue(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), _T("RegisteredOwner"), RRF_RT_ANY, NULL, osRegisteredOwner, size);
}

void getOsSystemroot(TCHAR osSystemroot[], LPDWORD size) {
	RegGetValue(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), _T("SystemRoot"), RRF_RT_ANY, NULL, osSystemroot, size);
}

void getOsBuild(TCHAR osBuild[], LPDWORD size) {
	RegGetValue(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), _T("CurrentBuild"), RRF_RT_ANY, NULL, osBuild, size);
}

void getProcessorInfo(LPWORD processorArchitecture, LPDWORD numberOfProcessors) {
	SYSTEM_INFO sysInfo;
	GetNativeSystemInfo(&sysInfo);
	*processorArchitecture = sysInfo.wProcessorArchitecture;
	*numberOfProcessors = sysInfo.dwNumberOfProcessors;
}

int getManufacturer(BSTR* manuf) {
    using defer = std::shared_ptr<void>;

    HRESULT hres;
    // Initialize COM.
    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres))
    {
        return 1;
    }

    // Initialize 
    hres = CoInitializeSecurity(
        NULL,
        -1,      // COM negotiates service                  
        NULL,    // Authentication services
        NULL,    // Reserved
        RPC_C_AUTHN_LEVEL_DEFAULT,    // authentication
        RPC_C_IMP_LEVEL_IMPERSONATE,  // Impersonation
        NULL,             // Authentication info 
        EOAC_NONE,        // Additional capabilities
        NULL              // Reserved
    );

    defer unInitialize(nullptr, [](...) { CoUninitialize(); });
    if (FAILED(hres))
    {
        //CoUninitialize();
        return 1;
    }

    // Obtain the initial locator to Windows Management
    // on a particular host computer.
    IWbemLocator* pLoc = 0;

    hres = CoCreateInstance(
        CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator, (LPVOID*)&pLoc);

    if (FAILED(hres))
    {
        //CoUninitialize();
        return 1;
    }

    IWbemServices* pSvc = 0;

    // Connect to the root\cimv2 namespace with the
    // current user and obtain pointer pSvc
    // to make IWbemServices calls.

    hres = pLoc->ConnectServer(

        _bstr_t(L"ROOT\\CIMV2"), // WMI namespace
        NULL,                    // User name
        NULL,                    // User password
        0,                       // Locale
        NULL,                    // Security flags                 
        0,                       // Authority       
        0,                       // Context object
        &pSvc                    // IWbemServices proxy
    );

    defer releasePloc(nullptr, [&](...) { pLoc->Release(); });

    if (FAILED(hres))
    {
        //pLoc->Release();
        //CoUninitialize();
        return 1;
    }

    // Set the IWbemServices proxy so that impersonation
    // of the user (client) occurs.
    hres = CoSetProxyBlanket(

        pSvc,                         // the proxy to set
        RPC_C_AUTHN_WINNT,            // authentication service
        RPC_C_AUTHZ_NONE,             // authorization service
        NULL,                         // Server principal name
        RPC_C_AUTHN_LEVEL_CALL,       // authentication level
        RPC_C_IMP_LEVEL_IMPERSONATE,  // impersonation level
        NULL,                         // client identity 
        EOAC_NONE                     // proxy capabilities     
    );

    defer releasePsvc(nullptr, [&](...) { pSvc->Release(); });

    if (FAILED(hres))
    {
        //pSvc->Release();
        //pLoc->Release();
        //CoUninitialize();
        return 1;
    }


    // Use the IWbemServices pointer to make requests of WMI. 
    // Make requests here:

    // For example, query for all the running processes
    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pSvc->ExecQuery(
        bstr_t("WQL"),
        bstr_t("SELECT * FROM Win32_Bios"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator);

    if (FAILED(hres))
    {
        //pSvc->Release();
        //pLoc->Release();
        //CoUninitialize();
        return 1;
    }
    else
    {
        IWbemClassObject* pclsObj;
        ULONG uReturn = 0;

        // usually there will be more than one value.
        // however this query returns merely one.
        // TODO: change the loop - because there is merely a single value
        while (pEnumerator)
        {
            hres = pEnumerator->Next(WBEM_INFINITE, 1,
                &pclsObj, &uReturn);

            if (0 == uReturn)
            {
                break;
            }

            VARIANT vtProp;

            // Get the value of the Name property
            hres = pclsObj->Get(L"Manufacturer", 0, &vtProp, 0, 0);
            *manuf = vtProp.bstrVal;
            VariantClear(&vtProp);

            pclsObj->Release();
            pclsObj = NULL;
        }

    }

    defer releasePEnumerator(nullptr, [&](...) { pEnumerator->Release(); });

    //pSvc->Release();
    //pLoc->Release();
    //pEnumerator->Release();
    //CoUninitialize();
    return 0;
}