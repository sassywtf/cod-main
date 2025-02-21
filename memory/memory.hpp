#pragma once
#include <windows.h>
#include <psapi.h>
#include <vector>
#include <string>

#include "Util.hpp"

#include "../Util/Import.hpp"
#include "../Util/Shared.hpp"

#include "../Spoofer/Spoofer.hpp"

class Memory
{
public:
    static uintptr_t GameAddress();
    static address_t PatternScan(const char* signature);

    static bool IsValidPtr(void* ptr) {
        __try { volatile auto result = *(uintptr_t*)ptr; }
        __except (EXCEPTION_EXECUTE_HANDLER) { return false; }
        return true;
    }

    template <typename Ty>
    static Ty Read(uintptr_t address) {
        if (!IsValidPtr((void*)address)) return Ty();
        return *reinterpret_cast<Ty*>(address);
    }

    template <typename Ty>
    static void Write(uintptr_t address, Ty buffer) {
        if (IsValidPtr((void*)address))
            *reinterpret_cast<Ty*>(address) = buffer;
    }
};
