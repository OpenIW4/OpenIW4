#include "IWNet.hpp"

#include <utils/memory/memory.hpp>

//DONE : 0x00405BE0
bool IWNet_DNSResolved()
{
    return *(bool*)0x405BE5; //dnsLookupSucceeded
}
