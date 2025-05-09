#include "version/version.h"
#include <chrono>
#include <thread>
#include <Zydis.h>
#include <LightningScanner/LightningScanner.hpp>

//#pragma comment(lib, "LightningScanner/LightningScanner143md")

using namespace std;

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

    //ServerGameBalanceSettings ServerGameSettings::ToStruct(bool log)
    ZyanU8* funcAddr = LightningScanner::Scanner("33 D2 41 B8 94 05 00 00 E8").Find(hMod, sizeOfImage).Get<ZyanU8>();

    if (funcAddr)
    {
        ZyanU64 currentZydisAddr = (ZyanU64)funcAddr;
        ZyanUSize offset = 0;

        const int funcSize = 0x1500;    // It works as is and I'm too lazy to make something less dumb

        ZydisDisassembledInstruction instruction;
        while (ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, currentZydisAddr, funcAddr + offset, funcSize - offset, &instruction)))
        {
            offset += instruction.info.length;
            currentZydisAddr += instruction.info.length;

            if (instruction.info.mnemonic == ZydisMnemonic::ZYDIS_MNEMONIC_COMISS && 
                (instruction.operands[1].reg.value == ZydisRegister::ZYDIS_REGISTER_XMM6 || instruction.operands[1].reg.value == ZydisRegister::ZYDIS_REGISTER_XMM2))   // Scanning for patters probably would've been more efficient, but I wanted to try out Zydis for the first time.
            {
                DWORD oriProtect;
                VirtualProtect((void*)instruction.runtime_address, instruction.info.length, PAGE_EXECUTE_READWRITE, &oriProtect);

                FillMemory((void*)instruction.runtime_address, instruction.info.length, 0x90);

                VirtualProtect((void*)instruction.runtime_address, instruction.info.length, oriProtect, &oriProtect);
            }

            if (instruction.info.mnemonic == ZydisMnemonic::ZYDIS_MNEMONIC_RET)
                break;
        }
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

