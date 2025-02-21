#include <windows.h>
#include <iostream>

#include "MinHook/MinHook.h"

#include "Util/Import.hpp"
#include "Util/Shared.hpp"

#include "Memory/Memory.hpp"
#include "Spoofer/Spoofer.hpp"

#include "SDK/Structs.hpp"
#include "SDK/Engine.hpp"

#include "Hooks/R_AddDObjToScene.hpp"

bool __stdcall DllMain(void* hinstDLL, std::uint32_t dwReason, void* lpReserved) {
    if (dwReason != DLL_PROCESS_ATTACH) return true;

    Detail::CodeCave = Memory::PatternScan("41 FF 26");
    if (!Detail::CodeCave) return false;

    Shared::R_AddDObjToScene_offs = Memory::PatternScan("E8 ? ? ? ? 48 81 C3 48 01 00 00 48 83 EE 01").SelfJmp(1);
    if (!Shared::R_AddDObjToScene_offs) return false;

    if (MH_Initialize() != MH_OK) return false;

    if (MH_CreateHook((void*)Shared::R_AddDObjToScene_offs, (void*)Hooks::R_AddDObjToScene, (void**)&Hooks::O_R_AddDObjToScene) != MH_OK) return false;

    if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) return false;

    return true;
}