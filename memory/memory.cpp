#include "Memory.hpp"

uintptr_t Memory::GameAddress() {
    static uintptr_t address = 0;
    if (!address)
        address = Read<uintptr_t>(__readgsqword(0x60) + 0x10);
    return address;
}

address_t Memory::PatternScan(const char* signature) {
    auto moduleHandle = (HMODULE)GameAddress();
    if (!moduleHandle) return {};

    static auto PatternToByte = [](const char* pattern) {
        auto bytes = std::vector<int>{};
        const auto start = const_cast<char*>(pattern);
        const auto end = const_cast<char*>(pattern) + strlen(pattern);

        for (auto current = start; current < end; ++current) {
            if (*current == '?') {
                ++current;
                if (*current == '?') ++current;
                bytes.push_back(-1);
            }
            else {
                bytes.push_back(strtoul(current, &current, 16));
            }
        }
        return bytes;
        };

    MODULEINFO moduleInfo{};
    import_(K32GetModuleInformation)(import_(GetCurrentProcess)(), moduleHandle, &moduleInfo, sizeof(MODULEINFO));
    const auto imageSize = moduleInfo.SizeOfImage;
    if (!imageSize) return {};

    auto patternBytes = PatternToByte(signature);
    auto imageBytes = reinterpret_cast<byte*>(moduleHandle);
    auto signatureSize = patternBytes.size();
    auto signatureBytes = patternBytes.data();

    PSAPI_WORKING_SET_EX_INFORMATION wsInfo;
    SIZE_T pageSize = 0x1000;

    for (byte* currentAddress = imageBytes; currentAddress < imageBytes + imageSize; currentAddress += pageSize) {
        wsInfo.VirtualAddress = reinterpret_cast<PVOID>(currentAddress);

        if (!import_(K32QueryWorkingSetEx)(import_(GetCurrentProcess)(), &wsInfo, sizeof(wsInfo)))
            continue;

        if (!wsInfo.VirtualAttributes.Valid)
            continue;

        unsigned long protection = wsInfo.VirtualAttributes.Win32Protection;
        if (protection != PAGE_EXECUTE_READ && protection != PAGE_EXECUTE_READWRITE)
            continue;

        for (const byte* currentByte = currentAddress; currentByte < currentAddress + pageSize - signatureSize; ++currentByte) {
            bool match = true;
            for (size_t i = 0; i < signatureSize; ++i) {
                if (signatureBytes[i] != -1 && signatureBytes[i] != currentByte[i]) {
                    match = false;
                    break;
                }
            }

            if (match) {
                return address_t(reinterpret_cast<uintptr_t>(currentByte));
            }
        }
    }

    return {};
}
