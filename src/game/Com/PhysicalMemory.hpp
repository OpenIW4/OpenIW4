#pragma once

#include "../stdafx.hpp"
#include "../defs.hpp"

#define MAX_PHYSICAL_MEMORY 0x12C00000

static PhysicalMemory* g_mem = reinterpret_cast<PhysicalMemory*>(0x64663F8);
