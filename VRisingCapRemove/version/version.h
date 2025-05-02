#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

HINSTANCE mHinst = 0, mHinstDLL = 0;

extern "C" UINT_PTR mProcs[17] = {0};

LPCSTR mImportNames[] = {
  "GetFileVersionInfoA",
  "GetFileVersionInfoByHandle",
  "GetFileVersionInfoExA",
  "GetFileVersionInfoExW",
  "GetFileVersionInfoSizeA",
  "GetFileVersionInfoSizeExA",
  "GetFileVersionInfoSizeExW",
  "GetFileVersionInfoSizeW",
  "GetFileVersionInfoW",
  "VerFindFileA",
  "VerFindFileW",
  "VerInstallFileA",
  "VerInstallFileW",
  "VerLanguageNameA",
  "VerLanguageNameW",
  "VerQueryValueA",
  "VerQueryValueW",
};

BOOL SetupWrapper(HINSTANCE hInst)
{
    //char sys[MAX_PATH];
    //GetSystemDirectoryA(sys, sizeof(sys));

    mHinst = hInst;
    mHinstDLL = LoadLibraryA("C:\\Windows\\System32\\version.dll");
    if (!mHinstDLL) 
    {
        return FALSE;
    }

    for (int i = 0; i < 17; ++i) {
        mProcs[i] = (UINT_PTR)GetProcAddress(mHinstDLL, mImportNames[i]);
    }

    return TRUE;
}

extern "C" void GetFileVersionInfoA_wrapper();
extern "C" void GetFileVersionInfoByHandle_wrapper();
extern "C" void GetFileVersionInfoExA_wrapper();
extern "C" void GetFileVersionInfoExW_wrapper();
extern "C" void GetFileVersionInfoSizeA_wrapper();
extern "C" void GetFileVersionInfoSizeExA_wrapper();
extern "C" void GetFileVersionInfoSizeExW_wrapper();
extern "C" void GetFileVersionInfoSizeW_wrapper();
extern "C" void GetFileVersionInfoW_wrapper();
extern "C" void VerFindFileA_wrapper();
extern "C" void VerFindFileW_wrapper();
extern "C" void VerInstallFileA_wrapper();
extern "C" void VerInstallFileW_wrapper();
extern "C" void VerLanguageNameA_wrapper();
extern "C" void VerLanguageNameW_wrapper();
extern "C" void VerQueryValueA_wrapper();
extern "C" void VerQueryValueW_wrapper();
