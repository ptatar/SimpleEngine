#pragma once

#include "Types.hpp"

namespace engine
{
    Int64 StringToInt64(const char* str, const char** endPtr = nullptr);

    Uint32 StringLen(const char* str);
} // namespace engine
