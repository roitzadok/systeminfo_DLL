#pragma once
#include "windows.h"
#define _WIN32_DCOM
#include <comdef.h>

#ifdef SYSTEMINFOLIBRARY_EXPORTS
#define SYSTEMINFOLIBRARY_API __declspec(dllexport)
#else
#define SYSTEMINFOLIBRARY_API __declspec(dllimport)
#endif

/*
<summary> gets the disk's total size </summary>
<returns> total number of bytes </returns>
*/
extern "C" SYSTEMINFOLIBRARY_API unsigned __int64 getTotalDiskSpace();

/*
<summary> gives info regarding the free and total bytes on the disk </summary>
<param name="FreeBytesAvailableToCaller"> pointer that will recieve the FreeBytesAvailableToCaller</param>
<param name="TotalNumberOfBytes"> pointer that will recieve the total disk size in bytes </param>
<param name="TotalNumberOfFreeBytes"> pointer that will recieve the total number of free bytes </param>
<returns> void </returns>
*/
extern "C" SYSTEMINFOLIBRARY_API void getDiskInfo(unsigned __int64* FreeBytesAvailableToCaller, unsigned __int64* TotalNumberOfBytes, unsigned __int64* TotalNumberOfFreeBytes);

/*
<summary> gives info regarding the free and total bytes on the disk </summary>
<param name="hostname"> pointer that will recieve the machine's hostname </param>
<param name="bufferSize"> hostname's size. will be modified to the returned length </param>
<returns> void </returns>
*/
extern "C" SYSTEMINFOLIBRARY_API void getHostname(char hostname[], LPDWORD bufferSize);

/*
<summary> retrieves the os's version </summary>
<param name="osVersion"> pointer that will recieve the version </param>
<param name="size"> osVersion's size. will be modified to the returned length </param>
<returns> void </returns>
*/
extern "C" SYSTEMINFOLIBRARY_API void getOsVersion(TCHAR osVersion[], LPDWORD size);

/*
<summary> retrieves the os's registered owner from the registry </summary>
<param name="osRegisteredOwner"> pointer that will recieve the osRegisteredOwner string </param>
<param name="size"> osRegisteredOwner's size. will be modified to the returned length </param>
<returns> void </returns>
*/
extern "C" SYSTEMINFOLIBRARY_API void getOsRegisteredOwner(TCHAR osRegisteredOwner[], LPDWORD size);

/*
<summary> retrieves the system root path </summary>
<param name="osSystemroot"> pointer that will recieve the path </param>
<param name="size"> osSystemroot's size. will be modified to the returned length </param>
<returns> void </returns>
*/
extern "C" SYSTEMINFOLIBRARY_API void getOsSystemroot(TCHAR osSystemroot[], LPDWORD size);

/*
<summary> retrieves the os's build number </summary>
<param name="osBuild"> pointer that will recieve the build number </param>
<param name="size"> osBuild's size. will be modified to the returned length </param>
<returns> void </returns>
*/
extern "C" SYSTEMINFOLIBRARY_API void getOsBuild(TCHAR osBuild[], LPDWORD size);

/*
<summary> retrieves information regarding the computer's processors </summary>
<param name="processorArchitecture"> pointer that will recieve a number that symbolizes the processor's architecture (as it represented in the ProcessorArchitecture Enum) </param>
<param name="numberOfProcessors"> pointer that will recieve the number of cores the computer has </param>
<returns> void </returns>
*/
extern "C" SYSTEMINFOLIBRARY_API void getProcessorInfo(LPWORD processorArchitecture, LPDWORD numberOfProcessors);

/*
<summary> retrieves the computer's manufacturer from the wmi </summary>
<param name="manuf"> pointer that will recieve the manufacturer </param>
<returns> error code </returns>
*/
extern "C" SYSTEMINFOLIBRARY_API int getManufacturer(BSTR* manuf);