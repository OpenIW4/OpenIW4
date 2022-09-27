#pragma once

#include "../stdafx.hpp"

static bool dnsLookupSucceeded = *reinterpret_cast<bool*>(0x405BE5);

bool IWNet_DNSResolved();
