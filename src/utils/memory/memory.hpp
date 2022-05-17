#pragma once

#include <functional>

namespace memory
{
    template <typename T> inline std::function<T> call(std::uintptr_t callback)
    {
        return std::function<T>(reinterpret_cast<T*>(callback));
    }

    inline auto replace(std::uint32_t address, void* function) -> void
    {
        auto page_protection = 0ul;
        auto opcode = reinterpret_cast<unsigned char*>(address);

        if (*opcode != 0xE8)
            return;

        VirtualProtect(reinterpret_cast<void*>(address), 5, PAGE_EXECUTE_READWRITE, &page_protection);
        reinterpret_cast<unsigned long*>(opcode + 1)[0] = reinterpret_cast<unsigned long>(function) - reinterpret_cast<unsigned long>(opcode + 5);
        VirtualProtect(reinterpret_cast<void*>(address), 5, page_protection, &page_protection);
        FlushInstructionCache(GetCurrentProcess(), reinterpret_cast<void*>(address), 5);
    }

    template <typename T> inline void xor(std::uint32_t place, T value)
    {
        *reinterpret_cast<T*>(place) ^= value;
    }
}