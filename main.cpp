#include <Windows.h>
#include <iostream>
#include <vector>

uintptr_t GetPointerAddress(uintptr_t ptr, std::vector<int> offsets)
{
    uintptr_t addr = ptr;
    for (int offset : offsets)
    {
        addr = *reinterpret_cast<uintptr_t*>(addr);
        addr += offset;
    }
    return addr;
}

DWORD WINAPI MainThread(HMODULE hModule)
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    printf("Dll made by epiclyrizzy\n");

    Sleep(2000);
    FreeConsole();

    // Convert the string to wide characters
    LPCWSTR moduleName = L"GameAssembly.dll";
    uintptr_t moduleBase = reinterpret_cast<uintptr_t>(GetModuleHandleW(moduleName));

    wprintf(L"Module Base: 0x%llX\n", moduleBase);

    while (!GetAsyncKeyState(VK_END))
    {
        int* Grenades = reinterpret_cast<int*>(GetPointerAddress(moduleBase + 0x029546C8, { 0xB8, 0x20, 0x518 }));
        wprintf(L"Grenades Address: 0x%llX\n", reinterpret_cast<uintptr_t>(Grenades));

        if (Grenades != nullptr)
        {
            *Grenades = 1337;
            wprintf(L"Grenades modified to 1337\n");
        }

        Sleep(1000);
    }

    if (GetAsyncKeyState(VK_END))
    {
        fclose(f);
    }

    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(MainThread), hModule, 0, nullptr));
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
