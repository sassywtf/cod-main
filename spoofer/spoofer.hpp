#pragma once
#include <windows.h>
#include <iostream>
#include <type_traits>

#include "../Memory/Memory.hpp"

namespace Detail
{
    inline void* CodeCave = nullptr;

    extern "C" void* SpooferStub();

    template <typename Ret, typename... Args>
    static inline auto ShellcodeStubHelper(const void* shell, Args... args) -> Ret
    {
        auto fn = (Ret(*)(Args...))(shell);
        return fn(args...);
    }

    template <std::size_t Argc, typename>
    struct ArgumentRemapper
    {
        // At least 5 params
        template< typename Ret, typename First, typename Second, typename Third, typename Fourth, typename... Pack >
        static auto DoCall(const void* shell, void* shellParam, First first, Second second, Third third, Fourth fourth, Pack... pack) -> Ret
        {
            return ShellcodeStubHelper<Ret, First, Second, Third, Fourth, void*, void*, Pack...>(shell, first, second, third, fourth, shellParam, nullptr, pack...);
        }
    };

    template <std::size_t Argc>
    struct ArgumentRemapper<Argc, std::enable_if_t<Argc <= 4>>
    {
        // 4 or less params
        template< typename Ret, typename First = void*, typename Second = void*, typename Third = void*, typename Fourth = void* >
        static auto DoCall(const void* shell, void* shellParam, First first = First{}, Second second = Second{}, Third third = Third{}, Fourth fourth = Fourth{}) -> Ret
        {
            return ShellcodeStubHelper<Ret, First, Second, Third, Fourth, void*, void*>(shell, first, second, third, fourth, shellParam, nullptr);
        }
    };
}

template <typename Ret, typename... Args>
static inline auto SpoofCall(Ret(*fn)(Args...), Args... args) -> Ret
{
    struct ShellParams
    {
        const void* Trampoline;
        void* Function;
        void* Rbx;
    };

    ShellParams params{ Detail::CodeCave, reinterpret_cast<void*>(fn) };
    using Mapper = Detail::ArgumentRemapper<sizeof...(Args), void>;
    return Mapper::template DoCall<Ret, Args...>((const void*)&Detail::SpooferStub, &params, args...);
}
