#include "version/version.h"
#include <chrono>
#include <thread>
#include <LightningScanner/LightningScanner.hpp>
#include <MinHook/include/MinHook.h>

//#pragma comment(lib, "LightningScanner/LightningScanner143md")

using namespace std;

uint16_t(*SettingsClamp_Half_o)(float val, float min, float max, void* fieldName) = nullptr;
uint16_t hook_SettingsClamp_Half(float val, float min, float max, void* fieldName)
{
    min = 0;
    max = val;

    return SettingsClamp_Half_o(val, min, max, fieldName);
}

DWORD WINAPI ScanThread(LPVOID lpThreadParameter)
{
    HMODULE hMod = 0;
    while (!hMod)
    {
        hMod = GetModuleHandleA("GameAssembly.dll");    //  GameAssembly.dll is loaded after version.dll
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    const IMAGE_DOS_HEADER* DOSHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(hMod);
    const IMAGE_NT_HEADERS* NtHeaders = reinterpret_cast<IMAGE_NT_HEADERS*>(reinterpret_cast<long long>(hMod) + DOSHeader->e_lfanew);
    const DWORD sizeOfImage = NtHeaders->OptionalHeader.SizeOfImage;

    //SettingsClamp::Half(float value, float min, float max, string fieldName)
    void* funcAddr = LightningScanner::Scanner("40 57 48 83 EC 60 80 3D ?? ?? ?? ?? ?? 49 8B").Find(hMod, sizeOfImage).Get<void>();

    if (funcAddr)
    {
        MH_Initialize();
        MH_CreateHook(funcAddr, &hook_SettingsClamp_Half, (void**)&SettingsClamp_Half_o);
        MH_EnableHook(funcAddr);
    }
    
    return TRUE;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(nullptr, 0, ScanThread, hModule, 0, nullptr);
        SetupWrapper(hModule);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

