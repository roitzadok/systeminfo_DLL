#include <iostream>
#include "SysteminfoLibrary.h"
#include <windows.h>
#include <tchar.h>
#include <map>

#define _WIN32_DCOM
#include <comdef.h>
#include <Wbemidl.h>

#pragma comment(lib, "wbemuuid.lib")

int main()
{
	unsigned __int64 FreeBytesAvailableToCaller, TotalNumberOfBytes, TotalNumberOfFreeBytes;
	getDiskInfo(&FreeBytesAvailableToCaller, &TotalNumberOfBytes, &TotalNumberOfFreeBytes);
	printf("FreeBytesAvailableToCaller: %I64u\n", FreeBytesAvailableToCaller);
	printf("TotalNumberOfBytes: %I64u\n", TotalNumberOfBytes);
	printf("TotalNumberOfFreeBytes: %I64u\n", TotalNumberOfFreeBytes);
	
	DWORD size = 255;
	char b[255];
	getHostname(b, &size);
	printf("hostname: %s\n", b);


	TCHAR osVersion[255];
	size = 255;
	getOsVersion(osVersion, &size);
	_tprintf(_T("os version: %s\n"), osVersion);

	TCHAR osRegisteredOwner[255];
	size = 255;
	getOsRegisteredOwner(osRegisteredOwner, &size);
	_tprintf(_T("os Registered Owner: %s\n"), osRegisteredOwner);

	TCHAR osSystemRoot[255];
	size = 255;
	getOsSystemroot(osSystemRoot, &size);
	_tprintf(_T("os System Root: %s\n"), osSystemRoot);

	TCHAR osBuild[255];
	size = 255;
	getOsBuild(osBuild, &size);
	_tprintf(_T("os Build: %s\n"), osBuild);


	
	std::map<int, const char *> ProcessorArchitecture;
	ProcessorArchitecture[0] = "X86";	
	ProcessorArchitecture[5] = "Arm";
	ProcessorArchitecture[9] = "X64";
	ProcessorArchitecture[11] = "Neutral";
	ProcessorArchitecture[12] = "Arm64";
	ProcessorArchitecture[14] = "X86OnArm64";
	ProcessorArchitecture[65535] = "Unknown";

	DWORD numberOfProcessors;
	WORD processorArchitecture;
	getProcessorInfo(&processorArchitecture, &numberOfProcessors);
	printf("processor architecture: %s\n", ProcessorArchitecture[processorArchitecture]);
	printf("number of cores: %i\n", numberOfProcessors);

    BSTR manuf;
    getManufacturer(&manuf);
    _tprintf(_T("manuf: %s\n"), manuf);
}
