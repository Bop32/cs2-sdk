#include "pch.hpp"

#include <instance/instance.hpp>
#include <logger/logger.hpp>

#include <memory/memory.hpp>
#include <hooks/hooks.hpp>

#include <constants/constants.hpp>

#ifdef _WIN32
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#endif

void CInstance::Initialize() {
    SDK_LOG_PROLOGUE();

    while (!AppFullyLoaded()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }

    CMemory::Get().Initialize();
    CHooks::Get().Initialize();
}

void CInstance::Shutdown() {
    SDK_LOG_PROLOGUE();

    CHooks::Get().Shutdown();
}

bool CInstance::AppFullyLoaded() { return CMemory::GetModule(CConstants::NAVSYSTEM_LIB) != nullptr; }

void CInstance::FreeLibrary() {
    SDK_LOG_PROLOGUE();

#ifdef _WIN32
    HANDLE hThread = CreateThread(
        NULL, 0, [](LPVOID pData) -> DWORD { FreeLibraryAndExitThread((HMODULE)(pData), EXIT_SUCCESS); }, (HINSTANCE)&__ImageBase, 0, NULL);

    if (hThread != NULL) {
        CloseHandle(hThread);
    }
#else
    Dl_info info{};
    if (dladdr(reinterpret_cast<void*>(CInstance::Get), &info) != 0) {
        void* currLib = dlopen(info.dli_fname, RTLD_LAZY | RTLD_NOLOAD);
        if (currLib) {
            for (int i = 0; i < 2; ++i) {
                pthread_t thread;
                pthread_create(&thread, nullptr, reinterpret_cast<void* (*)(void*)>(dlclose), currLib);
                pthread_detach(thread);
            }
        }
    }
#endif
}
