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
        *reinterpret_cast<std::uint8_t*>(address) = 0xE9;
        *reinterpret_cast<std::uint32_t*>(address + 1) = ((std::uint32_t)function - address - 5);
    }

    template <typename T> inline void xor(std::uint32_t place, T value)
    {
        *reinterpret_cast<T*>(place) ^= value;
    }

    template<typename T> inline void set(std::int32_t address, T value)
    {
        *reinterpret_cast<T*>(address) = value;
    }

    template<typename T> inline void kill(T address)
    {
        set<std::int8_t>(address, 0xC3);
    }
}
